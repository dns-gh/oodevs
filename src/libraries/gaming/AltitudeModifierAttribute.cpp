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
#include "LocationPositions.h"
#include "clients_kernel/AltitudeModified.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

namespace
{
class LocationVisitor : public LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    LocationVisitor() : isPolygon_( false )
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
    virtual void VisitLines    ( const T_PointVector& points ) { points_ = points; isPolygon_ = false; }
    virtual void VisitRectangle( const T_PointVector& points ) { points_ = points; isPolygon_ = true;  }
    virtual void VisitPolygon  ( const T_PointVector& points ) { points_ = points; isPolygon_ = true;  }
    virtual void VisitCircle   ( const geometry::Point2f&, float ) {}
    virtual void VisitPoint    ( const geometry::Point2f& ) {}
    virtual void VisitPath     ( const geometry::Point2f&, const T_PointVector& ) {}
    virtual void VisitCurve    ( const T_PointVector& ) {}
    virtual void VisitText     ( const QString&, const QFont&, const geometry::Point2f& ) {}
    //@}

public:
    //! @name Member data
    //@{
    T_PointVector points_;
    bool isPolygon_;
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
        ModifyAltitude( 0 );
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
        const int height = attributes.altitude_modifier().height();
        if( height != height_ )
        {
            height_ = height;
            ModifyAltitude( height_ );
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
        const int height = attributes.altitude_modifier().height();
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
    LocationVisitor visitor;
    const Location_ABC* location = static_cast< const LocationPositions& >( entity_.Get< Positions >() ).GetLocation();
    if( location )
    {
        location->Accept( visitor );
        if( !visitor.points_.empty() )
        {
            detection_.SetAltitudeOffset( entity_.GetId(), visitor.points_, visitor.isPolygon_, static_cast< short >( heightOffset ) );
            controller_.Update( AltitudeModified( visitor.points_ ) );
        }
    }
}
