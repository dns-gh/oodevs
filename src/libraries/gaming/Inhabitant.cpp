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
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"

using namespace geometry;
using namespace kernel;

unsigned long Inhabitant::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const sword::PopulationCreation& message, Controllers& controllers, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::InhabitantType >& typeResolver, UrbanModel& model )
    : EntityImplementation< Inhabitant_ABC >( controllers.controller_, message.id().id(), QString( message.name().c_str() ) )
    , controllers_  ( controllers )
    , converter_    ( converter )
    , type_         ( typeResolver.Get( message.type().id() ) )
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
        livingUrbanObject_.push_back( &urbanObject );
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

    for( DictionaryExtensions::CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        std::string info = "Details/" + it->first;
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", info.c_str() ), it->second );
    }
}
// -----------------------------------------------------------------------------
// Name: Inhabitant::DoUpdate
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DoUpdate( const sword::PopulationUpdate& msg )
{
    if( msg.has_healthy()  )
        nNbrHealthyHumans_ = msg.healthy();
    if( msg.has_dead()  )
        nNbrDeadHumans_ = msg.dead();
    if( msg.has_wounded()  )
        nNbrWoundedHumans_ = msg.wounded();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::ComputeCenter
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::ComputeCenter()
{
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetPosition
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
geometry::Point2f Inhabitant::GetPosition( bool ) const
{
    geometry::Polygon2f poly;
    for ( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); it++ )
    {
        poly.Add( (*it )->Barycenter() );
    }
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
bool Inhabitant::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float adaptiveFactor ) const
{
    return false; //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsIn
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return false; //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetBoundingBox
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f Inhabitant::GetBoundingBox() const
{
    return geometry::Rectangle2f(); //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    //TODO
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
        {
            (*it)->Display( tools::translate( "Inhabitant", "Alive:" ), nNbrHealthyHumans_ )
                  .Display( tools::translate( "Inhabitant", "Dead:" ), nNbrDeadHumans_ )
                  .Display( tools::translate( "Inhabitant", "Wounded:" ), nNbrWoundedHumans_ );
        }
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

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const kernel::GlTools_ABC& tools ) const
{
    for ( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const gui::TerrainObjectProxy& object = **it;
        const geometry::Polygon2f* footprint =  object.GetFootprint();
        if( footprint )
        {
            const geometry::Polygon2f::T_Vertices& points = footprint->Vertices();
            tools.DrawConvexPolygon( points );
        }
    }
}
