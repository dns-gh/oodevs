// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Color.h"
#include "clients_kernel/Entity_ABC.h"
#include <boost/algorithm/string.hpp>

Color::Color()
{
    // NOTHING
}

Color::Color( const kernel::Entity_ABC& parent )
{
    if( const kernel::Color_ABC* color = parent.Retrieve< kernel::Color_ABC >() )
        if( color->IsOverriden() )
            color_ = color->GetColor();
}

Color::Color( xml::xistream& xis )
{
    ChangeColor( xis );
}

Color::~Color()
{
    // NOTHING
}

namespace
{
    boost::tuple< unsigned int, unsigned int, unsigned int > HexToRgb( const std::string& strColor )
    {
        unsigned int color;
        std::stringstream ss( strColor );
        ss >> std::hex >> color;
        unsigned int red = ( color >> 16 ) & 0xff;
        unsigned int green = ( color >> 8 ) & 0xff;
        unsigned int blue = color & 0xff;
        return boost::tuples::make_tuple( red, green, blue );
    }

    std::string RgbToHex( unsigned int red, unsigned int green, unsigned int blue )
    {
        std::string result = QColor( red, green, blue ).name().toStdString();
        boost::replace_all( result, "#", "0x" );
        return result;
    }
}

void Color::ChangeColor( xml::xistream& xis )
{
    const std::string color = xis.attribute< std::string >( "color", "" );
    if( !color.empty() )
        color_ = HexToRgb( color );
}

void Color::SerializeAttributes( xml::xostream& xos ) const
{
    if( color_ )
        xos << xml::attribute( "color",
            RgbToHex( color_->get< 0 >(), color_->get< 1 >(), color_->get< 2 >() ) );
}
