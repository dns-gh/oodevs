// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Drawing.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, const ASN1T_MsgShapeCreationRequest& asn, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( asn.shape.category )
    , color_    ( asn.shape.color )
    , template_ ( asn.shape.template_ )
{
    for( unsigned int i = 0; i < asn.shape.points.n; ++i )
        points_.push_back( asn.shape.points.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( xml::attribute< std::string >( xis, "category" ) )
    , color_    ( xml::attribute< std::string >( xis, "color" ) )
    , template_ ( xml::attribute< std::string >( xis, "template" ) )
{
    xis >> xml::list( "point", *this, &Drawing::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, const Drawing& rhs )
    : converter_( rhs.converter_ )
    , id_       ( id )
    , category_ ( rhs.category_ )
    , color_    ( rhs.color_ )
    , template_ ( rhs.template_ )
    , points_   ( rhs.points_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing::ReadPoint
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void Drawing::ReadPoint( xml::xistream& xis )
{
    ASN1T_CoordLatLong asn;
    geometry::Point2f point( xml::attribute< float >( xis, "x" ), xml::attribute< float >( xis, "y" ) );
    converter_.ConvertToGeo( point, asn );
    points_.push_back( asn );
}

// -----------------------------------------------------------------------------
// Name: Drawing::GetId
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
unsigned long Drawing::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Drawing::Update
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void Drawing::Update( const ASN1T_MsgShapeUpdateRequest& asn )
{
    if( asn.m.categoryPresent )
        category_ = asn.category;
    if( asn.m.colorPresent )
        color_ = asn.color;
    if( asn.m.template_Present )
        template_ = asn.template_;
    if( asn.m.pointsPresent )
    {
        points_.clear();
        for( unsigned int i = 0; i < asn.points.n; ++i )
            points_.push_back( asn.points.elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendCreation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::ShapeCreation message;
    message().oid = id_;
    message().shape.category    = category_.c_str();
    message().shape.color       = color_.c_str();
    message().shape.template_   = template_.c_str();
    message().shape.points.n    = points_.size();
    message().shape.points.elem = const_cast< ASN1T_CoordLatLong* >( &points_.front() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendUpdate
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendUpdate( dispatcher::ClientPublisher_ABC& publisher ) const
{
    // $$$$ SBO 2008-06-09: keep track of updated fields...
    messenger::ShapeUpdate message;
    message().oid = id_;
    message().m.categoryPresent  = 1; message().category  = category_.c_str();
    message().m.colorPresent     = 1; message().color     = color_.c_str();
    message().m.template_Present = 1; message().template_ = template_.c_str();
    message().m.pointsPresent    = 1;
    message().points.n           = points_.size();
    message().points.elem        = const_cast< ASN1T_CoordLatLong* >( &points_.front() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendFullState
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const
{
    SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendDestruction
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::ShapeDestruction message;
    message().oid = id_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Drawing::Serialize
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void Drawing::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "shape" )
            << xml::attribute( "category", category_ )
            << xml::attribute( "color", color_ )
            << xml::attribute( "template", template_ );
    std::for_each( points_.begin(), points_.end(), boost::bind( &Drawing::SerializePoint, this, _1, boost::ref( xos ) ) );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializePoint
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void Drawing::SerializePoint( const ASN1T_CoordLatLong& asn, xml::xostream& xos ) const
{
    // $$$$ AGE 2008-07-09: serializer en mgrs ?
    const geometry::Point2f point( converter_.ConvertToXY( asn ) );
    xos << xml::start( "point" )
            << xml::attribute( "x", point.X() )
            << xml::attribute( "y", point.Y() )
        << xml::end();
}
