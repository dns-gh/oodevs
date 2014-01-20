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
        const QString hintX = content[ 0 ].stripWhiteSpace();
        const QString hintY = content[ 1 ].stripWhiteSpace();

        float x, y;
        std::stringstream strX( hintX.toStdString() );
        strX >> x ;
        if( ! strX )
            return false;
        std::stringstream strY( hintY.toStdString() );
        strY >> y ;
        if( ! strY )
            return false;
        hint.append( hintX );
        hint.append( hintY );

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
    return input.split( ":" );
}
