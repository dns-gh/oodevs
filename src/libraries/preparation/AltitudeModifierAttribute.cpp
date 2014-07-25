// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AltitudeModifierAttribute.h"
#include "clients_kernel/AltitudeModified.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

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
AltitudeModifierAttribute::AltitudeModifierAttribute( gui::PropertiesDictionary& dictionary, kernel::DetectionMap& detection,
                                                      const kernel::Object_ABC& object, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : detection_  ( detection )
    , object_     ( object )
    , controllers_( controllers )
    , height_     ( 0, Units::shortMeters )
{
    CreateDictionary( dictionary, entity );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( xml::xistream& xis, kernel::DetectionMap& detection, const kernel::Object_ABC& object,
                                                      gui::PropertiesDictionary& dictionary, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : detection_  ( detection )
    , object_     ( object )
    , controllers_( controllers )
    , height_     ( 0, Units::shortMeters )
{
    CreateDictionary( dictionary, entity );
    SetHeight( xis.attribute< unsigned int >( "height" ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Display
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Altitude modifier:" ), height_ * Units::shortMeters );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SerializeObjectAttributes
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "altitude-modifier" )
            << xml::attribute( "height", height_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::NotifyUpdated
// Created: JSR 2011-05-18
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::NotifyUpdated( const AltitudeModifierAttribute& attribute )
{
    if( &attribute == this )
    {
        LocationVisitor visitor;
        object_.Get< Positions >().Accept( visitor );
        if( !visitor.points_.empty() )
        {
            detection_.SetAltitudeOffset( object_.GetId(), visitor.points_, visitor.isPolygon_, static_cast< short >( height_.value_ ) );
            controllers_.controller_.Update( AltitudeModified( visitor.points_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::NotifyDeleted
// Created: JSR 2011-05-18
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::NotifyDeleted( const kernel::Object_ABC& object )
{
    if( &object == &object_ )
        SetHeight( 0 );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SetHeight
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SetHeight( unsigned int height )
{
    height_.value_ = height;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::BeginDrag
// Created: JSR 2012-05-09
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::BeginDrag() const
{
    if( !dragLocation_.empty() )
        return;
    LocationVisitor visitor;
    object_.Get< Positions >().Accept( visitor );
    dragLocation_ = visitor.points_;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::EndDrag
// Created: JSR 2012-05-09
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::EndDrag() const
{
    LocationVisitor visitor;
    object_.Get< Positions >().Accept( visitor );
    if( !dragLocation_.empty() && !visitor.points_.empty() && dragLocation_ != visitor.points_ )
    {
        detection_.SetAltitudeOffset( object_.GetId(), visitor.points_, visitor.isPolygon_, static_cast< short >( height_.value_ ) );
        controllers_.controller_.Update( AltitudeModified( dragLocation_ ) );
        controllers_.controller_.Update( AltitudeModified( visitor.points_ ) );
    }
    dragLocation_.clear();
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::CreateDictionary
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "AltitudeModifierAttribute", "Info/Altitude modifier/Height" ), height_ );
}
