// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Inhabitant.h"
#include "Tools.h"
#include "UrbanModel.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include <boost/foreach.hpp>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const sword::PopulationCreation& message, Controllers& controllers, const tools::Resolver_ABC< kernel::InhabitantType >& typeResolver, UrbanModel& model )
    : EntityImplementation< Inhabitant_ABC >( controllers.controller_, message.id().id(), QString( message.name().c_str() ) )
    , controllers_      ( controllers )
    , type_             ( typeResolver.Get( message.type().id() ) )
    , nNbrHealthyHumans_( 0 )
    , nNbrDeadHumans_   ( 0 )
    , nNbrWoundedHumans_( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.id().id() );
    if( message.has_extension() )
    {
        for( int i = 0; i < message.extension().entries_size(); ++i )
            extensions_[ message.extension().entries( i ).name() ] = message.extension().entries( i ).value();
    }
    for( int i = 0; i < message.blocks_size(); ++i )
    {
        int id = message.blocks( i ).id(); 
        gui::TerrainObjectProxy& urbanObject = model.GetObject( id );
        livingUrbanObject_[ id ] = &urbanObject;
    }
    CreateDictionary( controllers.controller_ );
    RegisterSelf( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Inhabitant& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Name" ), self.name_ );
    BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
    {
        std::string info = "Details/" + extension.first;
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", info.c_str() ), extension.second );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DoUpdate
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DoUpdate( const sword::PopulationUpdate& msg )
{
    if( msg.has_healthy() )
        nNbrHealthyHumans_ = msg.healthy();
    if( msg.has_dead() )
        nNbrDeadHumans_ = msg.dead();
    if( msg.has_wounded() )
        nNbrWoundedHumans_ = msg.wounded();
    for( int i = 0; i < msg.occupations_size(); ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& occupation = msg.occupations( i );
        unsigned int id = occupation.block().id();
        CIT_UrbanObjectVector it = livingUrbanObject_.find( id );
        if( it != livingUrbanObject_.end() )
            it->second->UpdateHumans( name_.ascii(), occupation.number() );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const GlTools_ABC& tools ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const gui::TerrainObjectProxy& object = *it->second;
        const geometry::Polygon2f* footprint = object.GetFootprint();
        if( footprint )
            tools.DrawConvexPolygon( footprint->Vertices() );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetPosition
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
geometry::Point2f Inhabitant::GetPosition( bool ) const
{
    geometry::Polygon2f poly;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        poly.Add( it->second->Barycenter() );
    return poly.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetHeight
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
float Inhabitant::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsAt
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::IsAt( const geometry::Point2f& /*pos*/, float /*precision*/, float /*adaptiveFactor*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsIn
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::IsIn( const geometry::Rectangle2f& /*rectangle*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetBoundingBox
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f Inhabitant::GetBoundingBox() const
{
    geometry::Rectangle2f box;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const geometry::Polygon2f* polygon = it->second->GetFootprint();
        if( polygon )
            BOOST_FOREACH( const geometry::Polygon2f::T_Vertices::value_type& point, polygon->Vertices() )
                box.Incorporate( point );
    }
    return box;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Accept( kernel::LocationVisitor_ABC& /*visitor*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInTooltip
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( (Inhabitant_ABC*)this ).End();
    displayer.Display( tools::translate( "Inhabitant", "Alive:" ), nNbrHealthyHumans_ );
    displayer.Display( tools::translate( "Inhabitant", "Wounded:" ), nNbrWoundedHumans_ );
    displayer.Display( tools::translate( "Inhabitant", "Dead:" ), nNbrDeadHumans_ );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInSummary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    const_cast< Inhabitant* >( this )->displayers_.insert( &displayer );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::NotifyUpdated
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::NotifyUpdated( const Simulation::sEndTick& /*tick*/ )
{
    if( !displayers_.empty() )
    {
        for( std::set< kernel::Displayer_ABC* >::iterator it = displayers_.begin(); it != displayers_.end(); ++it )
            (*it)->Display( tools::translate( "Inhabitant", "Alive:" ), nNbrHealthyHumans_ )
                  .Display( tools::translate( "Inhabitant", "Dead:" ), nNbrDeadHumans_ )
                  .Display( tools::translate( "Inhabitant", "Wounded:" ), nNbrWoundedHumans_ );
        displayers_.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::CanAggregate
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::CanAggregate() const
{
    return false;
}
