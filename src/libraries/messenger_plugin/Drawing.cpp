// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Drawing.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, const sword::ShapeCreationRequest& asn, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( asn.shape().category() )
    , color_    ( asn.shape().color() )
    , pattern_  ( asn.shape().pattern() )
{
    for( int i = 0; i < asn.shape().points().elem_size(); ++i )
        points_.push_back( asn.shape().points().elem(i) );
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( xis.attribute< std::string >( "category" ) )
    , color_    ( xis.attribute< std::string >( "color" ) )
    , pattern_  ( xis.attribute< std::string >( "template" ) )
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
    , pattern_  ( rhs.pattern_ )
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
    sword::MsgCoordLatLong asn;
    geometry::Point2f point( xis.attribute< float >( "x" ), xis.attribute< float >( "y" ) );
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
void Drawing::Update( const sword::ShapeUpdateRequest& asn )
{
    if( asn.has_category() )
        category_ = asn.category();
    if( asn.has_color() )
        color_ = asn.color();
    if( asn.has_template_() )
        pattern_ = asn.template_();
    if( asn.has_points() )
    {
        points_.clear();
        for( int i = 0; i < asn.points().elem_size(); ++i )
            points_.push_back( asn.points().elem(i) );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendCreation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    plugins::messenger::ShapeCreation message;
    message().mutable_id()->set_id( id_ );
    message().mutable_shape()->set_category( category_ );
    message().mutable_shape()->set_color( color_ );
    message().mutable_shape()->set_pattern( pattern_ );
    ::sword::MsgCoordLatLongList* points = message().mutable_shape()->mutable_points(); // required even if empty
    for (T_Points::const_iterator iter(points_.begin()); iter != points_.end(); ++iter)
        *points->add_elem() = *iter;        //const_cast< MsgCoordLatLong* >( &points_.front() );
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
    message().mutable_id()->set_id( id_ );
    message().set_category( category_ );
    message().set_color( color_ );
    message().set_template_( pattern_ );
    for (T_Points::const_iterator iter(points_.begin()); iter != points_.end(); ++iter)
        *message().mutable_points()->add_elem() = *iter;
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
    plugins::messenger::ShapeDestruction message;
    //ShapeDestruction message;
    message().mutable_id()->set_id( id_ );
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
            << xml::attribute( "template", pattern_ );
    std::for_each( points_.begin(), points_.end(), boost::bind( &Drawing::SerializePoint, this, _1, boost::ref( xos ) ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializePoint
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void Drawing::SerializePoint( const sword::MsgCoordLatLong& asn, xml::xostream& xos ) const
{
    // $$$$ AGE 2008-07-09: serializer en mgrs ?
    const geometry::Point2f point( converter_.ConvertToXY( asn ) );
    xos << xml::start( "point" )
            << xml::attribute( "x", point.X() )
            << xml::attribute( "y", point.Y() )
        << xml::end;
}
