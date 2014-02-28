// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Wgs84DdParser.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"

#include <boost/lexical_cast.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser constructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DdParser::Wgs84DdParser( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Wgs84DdParser destructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DdParser::~Wgs84DdParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::Parse
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
bool Wgs84DdParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const
{
    try
    {
        if( content.size() != 2 )
            return false;
        hint.clear();
        for( auto it = content.begin(); it != content.end(); ++it )
            hint << it->stripWhiteSpace();
        const float y = boost::lexical_cast< float >( hint[0].toStdString() );
        const float x = boost::lexical_cast< float >( hint[1].toStdString() );
        const geometry::Point2d point( x, y );
        result = converter_.ConvertFromGeo( point );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& Wgs84DdParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList()
            << tools::translate( "gui::LocationEditorBox", "Lat" )
            << tools::translate( "gui::LocationEditorBox", "Long" ),
        QList< int >() << INT_MAX << INT_MAX,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList Wgs84DdParser::Split( const QString& input ) const
{
    auto list = input.split( ":" );
    std::swap( list[0], list[1] );
    return list;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string Wgs84DdParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return converter_.GetStringPosition( position, eCoordinateSystem_Wgs84Dd );
}
