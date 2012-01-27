// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UndergroundAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace kernel;

std::map< std::string, QColor > UndergroundAttribute::undergroundColors_;

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( PropertiesDictionary& dico, Controller& controller )
    : controller_( controller )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( xml::xistream& xis, Controller& controller, PropertiesDictionary& dico )
    : controller_( controller )
{
    std::string strColor;
    xis >> xml::attribute( "network", network_ )
        >> xml::optional >> xml::attribute( "color", strColor );
    if( !strColor.empty() && undergroundColors_.find( network_ ) == undergroundColors_.end() )
    {
        unsigned int color;
        std::stringstream ss( strColor );
        ss >> std::hex >> color;
        undergroundColors_[ network_ ] = QColor( ( color >> 16 ) & 0xff, ( color >> 8 ) & 0xff, color & 0xff );
    }
    controller_.Update( *static_cast< UndergroundAttribute_ABC* >( this ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute destructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::~UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Purge
// Created: JSR 2012-01-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::Purge()
{
    undergroundColors_.clear();
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::GetNetwork
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
const std::string& UndergroundAttribute::GetNetwork() const
{
    return network_;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::GetOverridenColor
// Created: JSR 2012-01-11
// -----------------------------------------------------------------------------
const QColor* UndergroundAttribute::GetOverridenColor() const
{
    std::map< std::string, QColor >::const_iterator it = undergroundColors_.find( network_);
    if( it != undergroundColors_.end() )
        return &it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::OverrideColor
// Created: JSR 2012-01-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::OverrideColor( const QColor& color ) const
{
    undergroundColors_[ network_ ] = color;
    controller_.Update( *static_cast< const UndergroundAttribute_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::IsActivated
// Created: JSR 2011-07-18
// -----------------------------------------------------------------------------
bool UndergroundAttribute::IsActivated() const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Display
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Underground network:" ), network_ );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SerializeAttributes
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "underground" )
            << xml::attribute( "network", network_ );
    std::map< std::string, QColor >::const_iterator it = undergroundColors_.find( network_);
    if( it != undergroundColors_.end() )
    {
        std::string color = it->second.name().ascii();
        boost::replace_all( color, "#", "0x" );
        xos << xml::attribute( "color", color );
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SetNetwork
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
void UndergroundAttribute::SetNetwork( const std::string& network )
{
    network_ = network;
    controller_.Update( *static_cast< UndergroundAttribute_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::CreateDictionary
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "UndergroundAttribute", "Info/Underground/Network" ), network_ );
}
