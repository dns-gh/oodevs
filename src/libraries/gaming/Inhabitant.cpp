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
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const MsgsSimToClient::MsgPopulationCreation& message, Controllers& controllers, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::InhabitantType >& typeResolver, UrbanModel& model )
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
        DictionaryExtensions* extensions = new DictionaryExtensions;
        for( int i = 0; i < message.extension().entries_size(); ++i )
            extensions->SetValue( message.extension().entries( i ).name(), message.extension().entries( i ).value() );
        Attach( *extensions );
    }

    for( int i = 0; i < message.blocks_size(); ++i )
    {
        int id = message.blocks( i ).id(); 
        gui::TerrainObjectProxy& urbanObject = model.GetObject( id );
        livingUrbanObject_.push_back( &urbanObject );
    }


    RegisterSelf( *this );
    Attach( *new PropertiesDictionary( controllers.controller_ ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Inhabitant::DoUpdate( const MsgsSimToClient::MsgPopulationUpdate& msg )
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
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::ComputeCenter
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Inhabitant::ComputeCenter()
{
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetPosition
// Created: AGE 2006-04-10
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
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float Inhabitant::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Inhabitant::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float adaptiveFactor ) const
{
    return false; //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Inhabitant::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return false; //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f Inhabitant::GetBoundingBox() const
{
    return geometry::Rectangle2f(); //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void Inhabitant::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInTooltip
// Created: AGE 2006-06-29
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
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    const_cast< Inhabitant* >( this )->displayers_.insert( &displayer );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::NotifyUpdated
// Created: JSR 2010-05-20
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
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool Inhabitant::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-02
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
