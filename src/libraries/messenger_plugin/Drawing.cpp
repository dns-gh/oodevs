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
Drawing::Drawing( unsigned int id, const sword::Shape& msg, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( msg.category() )
    , color_    ( QColor( msg.color().red(), msg.color().green(), msg.color().blue() ).name().toStdString() )
    , pattern_  ( msg.pattern() )
    , name_     ( msg.has_name() ? msg.name() : pattern_ )
{
    if( msg.has_diffusion() )
        diffusion_ = msg.diffusion();
    for( int i = 0; i < msg.points().elem_size(); ++i )
        points_.push_back( msg.points().elem( i ) );
    if( msg.has_pen_style() )
        style_ = msg.pen_style();
    if( msg.has_text() )
        text_ = msg.text();
    if( msg.has_font() )
        font_ = msg.font();
}

namespace
{
    sword::EnumPenStyle ReadStyle( xml::xistream& xis )
    {
        const std::string style = xis.attribute( "style", "" );
        if( style == "dashed" )
            return sword::dashed;
        return sword::solid;
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
Drawing::Drawing( unsigned int id, xml::xistream& xis, const boost::optional< sword::Diffusion >& diffusion,
                  const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , id_       ( id )
    , category_ ( xis.attribute< std::string >( "category" ) )
    , color_    ( xis.attribute< std::string >( "color" ) )
    , pattern_  ( xis.attribute< std::string >( "template" ) )
    , name_     ( xis.attribute< std::string >( "name", pattern_ ) )
    , diffusion_( diffusion )
{
    xis >> xml::list( "point", *this, &Drawing::ReadPoint );
    if( xis.has_attribute( "style" ) )
        style_ = ReadStyle( xis );
    if( xis.has_child( "text" ) )
    {
        std::string text, font;
        xis >> xml::start( "text" )
                >> xml::attribute( "font", font )
                >> text
            >> xml::end;
        font_ = font;
        text_ = text;
    }
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
    , style_    ( rhs.style_ )
    , name_     ( rhs.name_ )
    , font_     ( rhs.font_ )
    , text_     ( rhs.text_ )
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
// Name: Drawing::GetPoints
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
// Name: Drawing::GetDiffusion
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
void Drawing::Update( const sword::ShapeUpdateRequest& msg )
{
    if( msg.has_category() )
        category_ = msg.category();
    if( msg.has_color() )
        color_ = QColor( msg.color().red(), msg.color().green(), msg.color().blue() ).name().toStdString();
    if( msg.has_pattern() )
        pattern_ = msg.pattern();
    if( msg.has_points() )
    {
        points_.clear();
        for( auto i = 0; i < msg.points().elem_size(); ++i )
            points_.push_back( msg.points().elem( i ) );
    }
    if( msg.has_name() )
        name_ = msg.name();
    if( msg.has_text() )
        text_ = msg.text();
    if( msg.has_font() )
        font_ = msg.font();
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendCreation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    plugins::messenger::ShapeCreation message;
    message().mutable_id()->set_id( id_ );
    SerializeShape( *message().mutable_shape() );
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
    SerializeShape( *message().mutable_shape() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializeShape
// Created: LGY 2014-07-23
// -----------------------------------------------------------------------------
void Drawing::SerializeShape( sword::Shape& shape ) const
{
    shape.set_category( category_ );
    QColor color;
    color.setNamedColor( color_.c_str() );
    shape.mutable_color()->set_red( color.red() );
    shape.mutable_color()->set_green( color.green() );
    shape.mutable_color()->set_blue( color.blue() );
    shape.set_pattern( pattern_ );
    shape.set_name( name_ );
    if( diffusion_ )
        *shape.mutable_diffusion() = *diffusion_;
    if( style_ )
        shape.set_pen_style( *style_ );
    if( text_ )
        shape.set_text( *text_ );
    if( font_ )
        shape.set_font( *font_ );
    sword::CoordLatLongList* points = shape.mutable_points(); // required even if empty
    for( auto iter = points_.begin(); iter != points_.end(); ++iter )
        *points->add_elem() = *iter;
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
            << xml::attribute( "name", name_ )
            << xml::attribute( "template", pattern_ );
    std::for_each( points_.begin(), points_.end(), boost::bind( &Drawing::SerializePoint, this, _1, boost::ref( xos ) ) );
    if( font_ && text_ )
        xos << xml::start( "text" )
                << xml::attribute( "font", *font_ )
                << *text_
            << xml::end
    << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializePoint
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void Drawing::SerializePoint( const sword::CoordLatLong& asn, xml::xostream& xos ) const
{
    xos << xml::start( "point" )
            << xml::attribute( "longitude", asn.longitude() )
            << xml::attribute( "latitude", asn.latitude() )
        << xml::end;
}
