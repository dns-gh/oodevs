// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "XyParser.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: XyParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
XyParser::XyParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XyParser destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
XyParser::~XyParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XyParser::Parse
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool XyParser::Parse( const QString& content, geometry::Point2f& result, QString& hint ) const
{
    hint = content;
    hint.replace( ',', ' ' );
    hint.replace( ';', ' ' );
    hint.replace( '[', ' ' );
    hint.replace( ']', ' ' );
    hint.replace( '(', ' ' );
    hint.replace( ')', ' ' );
    hint.stripWhiteSpace();

    std::stringstream str( hint.toStdString() );
    float x, y;
    str >> x >> y;
    if( ! str )
        return false;
    result.Set( x, y );
    return true;
}

// -----------------------------------------------------------------------------
// Name: XyParser::Parse
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
bool XyParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const
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

        result.Set( x, y );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: XyParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& XyParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList()
            << tools::translate( "gui::LocationEditorBox", "Y" )
            << tools::translate( "gui::LocationEditorBox", "X" ),
        QList< int >() << INT_MAX << INT_MAX,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: XyParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList XyParser::Split( const QString& input ) const
{
    return input.split( ":" );
}
