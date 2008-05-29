// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UtmParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UtmParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::UtmParser( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UtmParser destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::~UtmParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UtmParser::Parse
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool UtmParser::Parse( const QString& content, geometry::Point2f& result, QString& ) const
{
    // "31 NEA 00000 00000"
    try
    {
        result = converter_.ConvertToXY( std::string( content.ascii() ) );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}
