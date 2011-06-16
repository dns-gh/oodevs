// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_pch.h"
#include "PlatformDelegate.h"
#include "Platform.h"
#include "RoadFormation.h"
#include "DiamondFormation.h"
#include "PlatformVisitor_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <spatialcontainer/TerrainData.h>
#pragma warning( push, 0 )
#pragma warning( disable : 4180 )
#include <boost/bind.hpp>
#pragma warning( pop )

using namespace tic;

// -----------------------------------------------------------------------------
// Name: PlatformDelegate constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
PlatformDelegate::PlatformDelegate( dispatcher::Agent& holder, const kernel::CoordinateConverter_ABC& converter, float timeStep )
    : dispatcher::Observer< sword::UnitAttributes >     ( holder )
    , dispatcher::Observer< sword::UnitEnvironmentType >( holder )
    , dispatcher::Observer< sword::UnitPathFind >       ( holder )
    , holder_                                           ( holder )
    , converter_                                        ( converter )
    , onRoad_                                           ( false )
{
    CreatePlatforms( timeStep );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
PlatformDelegate::~PlatformDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::Notify
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void PlatformDelegate::Notify( const sword::UnitAttributes& message )
{
    if( message.has_equipment_dotations() )
        UpdatePlatforms( message );

    if( message.has_altitude() )
        std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &Platform::Update, _1, message ) );

    if( message.has_direction() )
    {
        const float angle = float( message.direction().heading() ) * std::acos( -1.f ) / 180.f;
        direction_ = geometry::Vector2f( std::sin( angle ), std::cos( angle ) );
        if( !onRoad_ )
            SortPlatforms();
    }
    if( message.has_position() )
        position_ = converter_.ConvertToXY( message.position() );
    UpdateFormation();
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::Notify
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void PlatformDelegate::Notify( const sword::UnitPathFind& message )
{
    path_.resize( 0 );
    for( int i = 0; i < message.path().location().coordinates().elem_size(); ++i )
        path_.push_back( converter_.ConvertToXY( message.path().location().coordinates().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::Notify
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void PlatformDelegate::Notify( const sword::UnitEnvironmentType& message )
{
    const unsigned int mask = TerrainData::motorway_  | TerrainData::largeroad_  | TerrainData::mediumroad_
                            | TerrainData::smallroad_ | TerrainData::bridge_;
    const bool nowOnRoad = ( message.linear() & mask ) != 0;
    if( onRoad_ != nowOnRoad )
    {
        SortPlatforms();
        onRoad_ = nowOnRoad;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::CreatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void PlatformDelegate::CreatePlatforms( float timeStep )
{
    const kernel::AgentType& type = holder_.GetType();
    tools::Iterator< const kernel::AgentComposition& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AgentComposition& component = it.NextElement();
        unsigned count = component.GetCount();
        while( count-- )
        {
            platforms_.push_back( new Platform( component.GetType(), timeStep ) );
            sorted_.push_back( &platforms_.back() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void PlatformDelegate::UpdatePlatforms( const sword::UnitAttributes& message )
{
    for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
        UpdatePlatforms( message.equipment_dotations().elem(i) );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void PlatformDelegate::UpdatePlatforms( const sword::EquipmentDotations_EquipmentDotation& message )
{
    sword::EquipmentDotations_EquipmentDotation copy( message );
    std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &Platform::Spread, _1, boost::ref( copy ) ) );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::UpdateFormation
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void PlatformDelegate::UpdateFormation()
{
    RoadFormation road( path_ );
    DiamondFormation diamond;
    Formation_ABC* current = onRoad_ ? &road : (Formation_ABC*) &diamond;
    current->Start( position_, direction_, sorted_.size() );
    std::for_each( sorted_.begin(), sorted_.end(), boost::bind( &Formation_ABC::Apply, current, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::Accept
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void PlatformDelegate::Accept( PlatformVisitor_ABC& visitor ) const
{
    std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &PlatformVisitor_ABC::AddPlatform, &visitor, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegate::SortPlatforms
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void PlatformDelegate::SortPlatforms()
{
    std::sort( sorted_.begin(), sorted_.end(),
        boost::bind( &Formation_ABC::Compare,
            boost::bind( &Platform::GetPosition, _1 ),
            boost::bind( &Platform::GetPosition, _2 ),
            position_, direction_ ) );
}
