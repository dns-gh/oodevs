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

       