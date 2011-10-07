// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AltitudeModifierAttribute.h"
#include "Simulation.h"
#include "Tools.h"
#include "clients_kernel/AltitudeModified.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Units.h"

using namespace kernel;

namespace
{
class LocationVisitor : public LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    LocationVisitor( geometry::Polygon2f& polygon )
        : polygon_( polygon )
    {
        // NOTHING
    }
    virtual ~LocationVisitor()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines    ( const T_PointVector& points ) { polygon_ = geometry::Polygon2f( points ); }
    virtual void VisitRectangle( const T_PointVector& points ) { polygon_ = geometry::Polygon2f( points ); }
    virtual void VisitPolygon  ( const T_PointVector& points ) { polygon_ = geometry::Polygon2f( points ); }
    virtual void VisitCircle   ( const geometry::Point2f&, float ) {}
    virtual void VisitPoint    ( const geometry::Point2f& ) {}
    virtual void VisitPath     ( const geometry::Point2f&, const T_PointVector& ) {}
    //@}

private:
    //! @name Member data
    //@{
    geometry::Polygon2f& polygon_;
    //@}
};
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( Controller& controller, kernel::DetectionMap& detection, const kernel::Entity_ABC& entity )
    : controller_      ( controller )
    , detection_       ( detection )
    , entity_          ( entity )
    , height_          ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Display
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Altitude modifier:" ), height_ * Units::meters );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DisplayInTooltip
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DisplayInSummary
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::NotifyDeleted
// Created: JSR 2011-05-18
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::NotifyDeleted( const kernel::Object_ABC& entity )
{
    if( static_cast< const Entity_ABC* >( &entity ) == &entity_ )
        ModifyAltitude( -height_ );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DoUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    if( !message.has_attributes() )
        return;
    const sword::ObjectAttributes& attributes = message.attributes();
    if( attributes.has_altitude_modifier() )
    {
        int height = attributes.altitude_modifier().height();
        if( height != height_ )
        {
            ModifyAltitude( height - height_ );
            height_ = height;
            controller_.Update( *static_cast< AltitudeModifierAttribute_ABC* >( this ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DoUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( !message.has_attributes() )
        return;
    const sword::ObjectAttributes& attributes = message.attributes();
    if( attributes.has_altitude_modifier() )
    {
        int height = attributes.altitude_modifier().height();
        if( height != height_ )
        {
            height_ = height;
            controller_.Update( *static_cast< AltitudeModifierAttribute_ABC* >( this ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::ModifyAltitude
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::ModifyAltitude( int heightOffset )
{
    geometry::Polygon2f polygon;
    LocationVisitor visitor( polygon );
    entity_.Get< Positions >().Accept( visitor );
    if( !polygon.IsEmpty() )
    {
        detection_.ModifyAltitude( polygon, static_cast< short >( heightOffset ) );
        controller_.Update( AltitudeModified( polygon ) );
    }
}
