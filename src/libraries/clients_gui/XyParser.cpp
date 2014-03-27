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
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: XyParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
XyParser::XyParser( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
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
bool XyParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool small ) const
{
    if( content.size() != 2 )
        return false;
    hint.clear();
    for( auto it = content.begin(); it != content.end(); ++it )
        hint << it->stripWhiteSpace();
    bool ok;
    const float y = content[0].toFloat( &ok );
    if( !ok )
        return false;
    const float x = content[1].toFloat( &ok );
    if( !ok )
        return false;
    result.Set( x, y );
    if( small )
    {
        hint[0] = QString::number( y, 'f', 2 );
        hint[1] = QString::number( x, 'f', 2 );
    }
    return true;
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

// -----------------------------------------------------------------------------
// Name: XyParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string XyParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return converter_.GetStringPosition( position, eCoordinateSystem_Local );
}
