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
#include "clients_gui/DrawingTypes.h"
#include "clients_gui/DrawingCategory.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controller& controller, const MsgsMessengerToClient::MsgShapeCreation& message, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controller, message.oid(), types.Get( QString( message.shape().category().c_str() ) ).Get( QString( message.shape().template_().c_str() ) ), QColor( QString( message.shape().color().c_str() ) ), proxy )
    , publisher_( publisher )
    , converter_( converter )
    , publishUpdate_( true )
{
    SetLocation( message.shape().points() );
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
void Drawing::SetLocation( const Common::MsgCoordLatLongList& list )
{
    std::auto_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    location.release();
    for( int i = 0; i < list.elem_size(); ++i )
        location_.AddPoint( converter_.ConvertToXY( list.elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: Drawing::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Create()
{
    plugins::messenger::ShapeCreationRequest message;
    message().mutable_shape()->set_category( style_.GetCategory().ascii() );
    std::string color = color_.name().ascii();
    message().mutable_shape()->set_color( color.c_str() );
    message().mutable_shape()->set_template_( style_.GetName().ascii() );
    SerializeLocation( *message().mutable_shape()->mutable_points() );
    message.Send( publisher_ );
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
        plugins::messenger::ShapeUpdateRequest message;
        message().set_oid( GetId() );
        message().set_category( style_.GetCategory().ascii() );
        std::string color = color_.name().ascii();
        message().set_color( color.c_str() );
        message().set_template_( style_.GetName().ascii() );
        SerializeLocation( *message().mutable_points() );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::DoUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::DoUpdate( const MsgsMessengerToClient::MsgShapeUpdate& message )
{
    publishUpdate_ = false;
     // $$$$ SBO 2008-06-09: can only change color and shape
    if( message.has_color()  )
        ChangeColor( QString( message.color().c_str() ) );
    if( message.has_points()  )
        SetLocation( message.points() );
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
                Common::MsgCoordLatLong latlong;
                converter_->ConvertToGeo( *it, latlong );
                points_.push_back( latlong );
            }
        }
        virtual void VisitRectangle( const T_PointVector& points )
        {
            VisitLines( points );
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            VisitLines( points );
            points_.pop_back();
        }
        virtual void VisitCircle( const geometry::Point2f& /*center*/, float /*radius*/ )
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
        std::vector< Common::MsgCoordLatLong > points_;
        const kernel::CoordinateConverter_ABC* converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializeLocation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::SerializeLocation( Common::MsgCoordLatLongList& list ) const
{
    Serializer serializer( converter_ );
    location_.Accept( serializer );
    if( serializer.points_.size() )
    {
        for( unsigned int i = 0; i < serializer.points_.size(); ++i )
            list.add_elem();
        std::memcpy( list.mutable_elem(), &serializer.points_.front(), list.elem_size() * sizeof( Common::MsgCoordLatLong ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Drawing::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
