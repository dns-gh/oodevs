// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ResourceNetworkType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType constructor
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
ResourceNetworkType::ResourceNetworkType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ) )
{
    std::string strColor;
    unsigned int color;
    xis >> xml::start( "color" )
            >> xml::attribute( "rgb", strColor )
        >> xml::end
        >> xml::start( "defaults" )
            >> xml::attribute( "production", defaultProduction_ )
        >> xml::end;
    std::stringstream colorStream( strColor );
    colorStream >> std::hex >> color;
    red_ = static_cast< float >( ( color >> 16 ) & 0xff ) / 255;
    green_ = static_cast< float >( ( color >> 8 ) & 0xff ) / 255;
    blue_ = static_cast< float >( color & 0xff ) / 255;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType destructor
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
ResourceNetworkType::~ResourceNetworkType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::GetName
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const std::string& ResourceNetworkType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::GetColor
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void ResourceNetworkType::GetColor( float& red, float& green, float& blue ) const
{
    red = red_;
    green = green_;
    blue = blue_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::GetDefaultProduction
// Created: JSR 2013-02-27
// -----------------------------------------------------------------------------
unsigned int ResourceNetworkType::GetDefaultProduction() const
{
    return defaultProduction_;
}
