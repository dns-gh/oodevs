// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "XyParser.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: XyParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
XyParser::XyParser() : numParameters_( 2 )
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
bool XyParser::Parse( QString content, geometry::Point2f& result, QString& hint ) const
{
    hint = content;
    hint.replace( ',', ' ' );
    hint.replace( ';', ' ' );
    hint.replace( '[', ' ' );
    hint.replace( ']', ' ' );
    hint.replace( '(', ' ' );
    hint.replace( ')', ' ' );
    hint.stripWhiteSpace();

    std::stringstream str( hint.ascii() );
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
bool XyParser::Parse( QString content, geometry::Point2f& result, QStringList& hint ) const
{
    QStringList listValue = QStringList::split( ":", content );
    QString hintX = listValue[ 0 ].stripWhiteSpace();
    QString hintY = listValue[ 1 ].stripWhiteSpace();

    float x, y;
    std::stringstream strX( hintX.ascii() );
    strX >> x ;
    if( ! strX )
        return false;
    std::stringstream strY( hintY.ascii() );
    strY >> y ;
    if( ! strY )
        return false;

    hint.append( hintX );
    hint.append( hintY );

    result.Set( x, y );
        return true;
}

// -----------------------------------------------------------------------------
// Name: XyParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int XyParser::GetNumberOfParameters() const
{
    return numParameters_;
}
       