// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "TicExtension.h"
#include "Platform.h"
#include "RoadFormation.h"
#include "DiamondFormation.h"
#include "PlatformVisitor_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <pathfind/TerrainData.h>
#pragma warning( push, 0 )
#pragma warning( disable : 4180 )
#include <boost/bind.hpp>
#pragma warning( pop )

using namespace plugins::tic;

// -----------------------------------------------------------------------------
// Name: TicExtension constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
TicExtension::TicExtension( dispatcher::Agent& holder, const kernel::CoordinateConverter_ABC& converter, float timeStep )
    : holder_   ( holder )
    , converter_( converter )
    , onRoad_   ( false )
{
    CreatePlatforms( timeStep );
}

// -----------------------------------------------------------------------------
// Name: TicExtension destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
TicExtension::~TicExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TicExtension::DoUpdate
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( message.has_dotation_eff_materiel() )
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
// Name: TicExtension::DoUpdate
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicExtension::DoUpdate( const MsgsSimToClient::MsgUnitPathFind& message )
{
    path_.resize( 0 );
    for( int i = 0; i < message.itineraire().location().coordinates().elem_size(); ++i )
        path_.push_back( converter_.ConvertToXY( message.itineraire().location().coordinates().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::DoUpdate
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::DoUpdate( const MsgsSimToClient::MsgUnitEnvironmentType& message )
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
// Name: TicExtension::CreatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::CreatePlatforms( float timeStep )
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
// Name: TicExtension::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::UpdatePlatforms( const MsgsSimToClient::MsgUnitAttributes& message )
{
    for( int i = 0; i < message.dotation_eff_materiel().elem_size(); ++i )
        UpdatePlatforms( message.dotation_eff_materiel().elem(i) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::UpdatePlatforms( const MsgsSimToClient::EquipmentDotations_EquipmentDotation& message )
{
    MsgsSimToClient::EquipmentDotations_EquipmentDotation copy( message );
    std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &Platform::Spread, _1, boost::ref( copy ) ) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::UpdateFormation
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicExtension::UpdateFormation()
{
    RoadFormation road( path_ );
    DiamondFormation diamond;
    Formation_ABC* current = onRoad_ ? &road : (Formation_ABC*) &diamond;
    current->Start( position_, direction_, sorted_.size() );
    std::for_each( sorted_.begin(), sorted_.end(), boost::bind( &Formation_ABC::Apply, current, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::Accept
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicExtension::Accept( PlatformVisitor_ABC& visitor ) const
{
    std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &PlatformVisitor_ABC::AddPlatform, &visitor, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::SortPlatforms
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void TicExtension::SortPlatforms()
{
    std::sort( sorted_.begin(), sorted_.end(),
        boost::bind( &Formation_ABC::Compare,
            boost::bind( &Platform::GetPosition, _1 ),
            boost::bind( &Platform::GetPosition, _2 ),
            position_, direction_ ) );
}
