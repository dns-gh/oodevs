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
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

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
    virtual void VisitLines    ( const T_PointVector& ) {}
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
AltitudeModifierAttribute::AltitudeModifierAttribute( kernel::PropertiesDictionary& dico, kernel::DetectionMap& detection, const kernel::Object_ABC& object, kernel::Controllers& controllers )
    : detection_  ( detection )
    , object_     ( object )
    , controllers_( controllers )
    , height_     ( 0, Units::meters )
    , oldHeight_  ( 0 )
{
    CreateDictionary( dico );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( xml::xistream& xis, kernel::DetectionMap& detection, const kernel::Object_ABC& object, kernel::PropertiesDictionary& dico, kernel::Controllers& controllers )
    : detection_  ( detection )
    , object_     ( object )
    , controllers_( controllers )
    , height_     ( 0, Units::meters )
    , oldHeight_  ( 0 )
{
    CreateDictionary( dico );
    controllers_.Register( *this );
    SetHeight( xis.attribute< unsigned int >( "height" ) );
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
             .Display( tools::translate( "Object", "Altitude modifier:" ), height_ * Units::meters );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SerializeAttributes
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SerializeAttributes( xml::xostream& xos ) const
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
    if( &attribute == this && oldHeight_ != height_.value_ )
    {
        ModifyAltitude( height_.value_ - oldHeight_ );
        oldHeight_ = height_.value_;
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
// Name: AltitudeModifierAttribute::CreateDictionary
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "AltitudeModifierAttribute", "Info/Altitude modifier/Height" ), height_ );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::ModifyAltitude
// Created: JSR 2011-05-18
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::ModifyAltitude( int heightOffset )
{
    geometry::Polygon2f polygon;
    LocationVisitor visitor( polygon );
    object_.Get< Positions >().Accept( visitor );
    if( !polygon.IsEmpty() )
    {
        detection_.ModifyAltitude( polygon, static_cast< short >( heightOffset ) );
        controllers_.controller_.Update( AltitudeModified( polygon ) );
    }
}
