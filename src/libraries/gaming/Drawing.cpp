// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Drawing.h"
#include "game_asn/MessengerSenders.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_gui/DrawingCategory.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "clients_kernel/ModelVisitor_ABC.h"

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controller& controller, const ASN1T_MsgShapeCreation& asn, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controller, asn.oid, types.Get( asn.shape.category ).Get( asn.shape.template_ ), QColor( asn.shape.color ), proxy )
    , publisher_( publisher )
    , converter_( converter )
    , publishUpdate_( true )
{
    SetLocation( asn.shape.points );
    DrawerShape::Create();
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controller& controller, const gui::DrawingTemplate& style, const QColor& color, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controller, 0, style, color, proxy )
    , publisher_( publisher )
    , converter_( converter )
    , publishUpdate_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controller& controller, xml::xistream& xis, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controller, 0, xis, types, proxy )
    , publisher_( publisher )
    , converter_( converter )
    , publishUpdate_( true )
{
    Create();
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing::SetLocation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SetLocation( const ASN1T_CoordLatLongList& list )
{
    std::auto_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    location.release();
    for( unsigned int i = 0; i < list.n; ++i )
        location_.AddPoint( converter_.ConvertToXY( list.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: Drawing::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Create()
{
    plugins::messenger::ShapeCreationRequest asn;
    asn().shape.category = style_.GetCategory().ascii();
    std::string color = color_.name().ascii();
    asn().shape.color = color.c_str();
    asn().shape.template_ = style_.GetName().ascii();
    SerializeLocation( asn().shape.points );
    asn.Send( publisher_ );
    CleanLocation( asn().shape.points );
    delete this;
}

// -----------------------------------------------------------------------------
// Name: Drawing::Update
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Update()
{
    DrawerShape::Update();
    if( publishUpdate_ )
    {
        plugins::messenger::ShapeUpdateRequest asn;
        asn().oid = GetId();
        asn().m.categoryPresent  = 1; asn().category  = style_.GetCategory().ascii();
        asn().m.colorPresent     = 1;
        std::string color = color_.name().ascii();
        asn().color = color.c_str();
        asn().m.template_Present = 1; asn().template_ = style_.GetName().ascii();
        asn().m.pointsPresent    = 1; SerializeLocation( asn().points );
        asn.Send( publisher_ );
        CleanLocation( asn().points );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::DoUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::DoUpdate( const ASN1T_MsgShapeUpdate& message )
{
    publishUpdate_ = false;
     // $$$$ SBO 2008-06-09: can only change color and shape
    if( message.m.colorPresent )
        ChangeColor( message.color );
    if( message.m.pointsPresent )
        SetLocation( message.points );
    publishUpdate_ = true;
}

namespace
{
    struct Serializer : public kernel::LocationVisitor_ABC
    {
        explicit Serializer( const kernel::CoordinateConverter_ABC& converter ) : converter_( &converter ) {}
        virtual void VisitLines( const T_PointVector& points )
        {
            for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
            {
                ASN1T_CoordLatLong latlong;
                converter_->ConvertToGeo( *it, latlong );
                points_.push_back( latlong );
            }
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            VisitLines( points );
            points_.pop_back();
        }
        virtual void VisitCircle( const geometry::Point2f& center, float radius )
        {
            // $$$$ SBO 2008-06-05: TODO
        }
        virtual void VisitPoint( const geometry::Point2f& point )
        {
            VisitLines( T_PointVector( 1, point ) );
        }
        virtual void VisitPath( const geometry::Point2f& , const T_PointVector& points )
        {
            VisitLines( points );
        }
        std::vector< ASN1T_CoordLatLong > points_;
        const kernel::CoordinateConverter_ABC* converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializeLocation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::SerializeLocation( ASN1T_CoordLatLongList& list ) const
{
    Serializer serializer( converter_ );
    location_.Accept( serializer );
    list.n = serializer.points_.size();
    if( list.n )
    {
        list.elem = new ASN1T_CoordLatLong[ list.n ];
        std::memcpy( list.elem, &serializer.points_.front(), list.n * sizeof( ASN1T_CoordLatLong ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::CleanLocation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::CleanLocation( ASN1T_CoordLatLongList& list ) const
{
    if( list.n )
        delete[] list.elem;
}

// -----------------------------------------------------------------------------
// Name: Drawing::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Drawing::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
