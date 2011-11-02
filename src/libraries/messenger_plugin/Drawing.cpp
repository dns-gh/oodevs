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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <QtGui/qcolor.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, const sword::ShapeCreationRequest& msg, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( msg.shape().category() )
    , color_    ( QColor( msg.shape().color().red(), msg.shape().color().green(), msg.shape().color().blue() ).name().ascii() )
    , pattern_  ( msg.shape().pattern() )
{
    if( msg.shape().has_diffusion() )
        diffusion_ = msg.shape().diffusion();
    for( int i = 0; i < msg.shape().points().elem_size(); ++i )
        points_.push_back( msg.shape().points().elem( i ) );
    if( msg.shape().has_text() )
        text_ = msg.shape().text();
    if( msg.shape().has_font() )
        font_ = msg.shape().font();
    if( msg.shape().has_font_size() )
        fontSize_ = msg.shape().font_size();
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, xml::xistream& xis, const boost::optional< sword::Diffusion >& diffusion, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( xis.attribute< std::string >( "category" ) )
    , color_    ( xis.attribute< std::string >( "color" ) )
    , pattern_  ( xis.attribute< std::string >( "template" ) )
    , diffusion_( diffusion )
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
    , diffusion_( rhs.diffusion_ )
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
    sword::CoordLatLong asn;
    if( xis.has_attribute( "x" ) && xis.has_attribute( "y" ) )
    {
        geometry::Point2f point( xis.attribute< float >( "x" ), xis.attribute< float >( "y" ) );
        converter_.ConvertToGeo( point, asn );
    }
    else
    {
        asn.set_longitude( xis.attribute< double >( "longitude" ) );
        asn.set_latitude( xis.attribute< double >( "latitude" ) );
    }
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
// Name: std::vector< sword::CoordLatLong >& Drawing::GetPoints
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
const std::vector< sword::CoordLatLong >& Drawing::GetPoints() const
{
    return points_;
}

// -----------------------------------------------------------------------------
// Name: Drawing::GetConverter
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
const kernel::CoordinateConverter_ABC& Drawing::GetConverter() const
{
    return converter_;
}

// -----------------------------------------------------------------------------
// Name: boost::optional< sword::Diffusion >& Drawing::GetDiffusion
// Created: JSR 2011-06-30
// -----------------------------------------------------------------------------
const boost::optional< sword::Diffusion >& Drawing::GetDiffusion() const
{
    return diffusion_;
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
        color_ = QColor( asn.color().red(), asn.color().green(), asn.color().blue() ).name().ascii();
    if( asn.has_pattern() )
        pattern_ = asn.pattern();
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
    QColor color;
    color.setNamedColor( color_.c_str() );
    message().mutable_shape()->mutable_color()->set_red( color.red() );
    message().mutable_shape()->mutable_color()->set_green( color.green() );
    message().mutable_shape()->mutable_color()->set_blue( color.blue() );
    message().mutable_shape()->set_pattern( pattern_ );
    if( diffusion_ )
        *message().mutable_shape()->mutable_diffusion() = *diffusion_;
    if( text_ )
        message().mutable_shape()->set_text( *text_ );
    if( font_ )
        message().mutable_shape()->set_font( *font_ );
    if( fontSize_ )
        message().mutable_shape()->set_font_size( *fontSize_ );
    sword::CoordLatLongList* points = message().mutable_shape()->mutable_points(); // required even if empty
    for( CIT_Points iter = points_.begin(); iter != points_.end(); ++iter )
        *points->add_elem() = *iter;        //const_cast< CoordLatLong* >( &points_.front() );
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
    message().mutable_shape()->set_category( category_ );
    QColor color;
    color.setNamedColor( color_.c_str() );
    message().mutable_shape()->mutable_color()->set_red( color.red() );
    message().mutable_shape()->mutable_color()->set_green( color.green() );
    message().mutable_shape()->mutable_color()->set_blue( color.blue() );
    message().mutable_shape()->set_pattern( pattern_ );
    for( CIT_Points iter = points_.begin(); iter != points_.end(); ++iter )
        *message().mutable_shape()->mutable_points()->add_elem() = *iter;
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
void Drawing::SerializePoint( const sword::CoordLatLong& asn, xml::xostream& xos ) const
{
    // $$$$ AGE 2008-07-09: serializer en mgrs ?
    const geometry::Point2f point( converter_.ConvertToXY( asn ) );
    xos << xml::start( "point" )
            << xml::attribute( "x", point.X() )
            << xml::attribute( "y", point.Y() )
        << xml::end;
}
