// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationParsers.h"
#include "UtmParser.h"
#include "XYParser.h"
#include "Wgs84DdParser.h"
#include "Wgs84DmsParser.h"
#include "clients_kernel/CoordinateSystems.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationParsers constructor
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParsers::LocationParsers( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , converter_( converter )
{
    parsers_[ kernel::CoordinateSystems::E_Mgrs ].reset( new UtmParser( controllers_, converter_ ) );
    parsers_[ kernel::CoordinateSystems::E_Local ].reset( new XyParser() );
    parsers_[ kernel::CoordinateSystems::E_Wgs84Dd ].reset( new Wgs84DdParser( converter_ ) );
    parsers_[ kernel::CoordinateSystems::E_Wgs84Dms ].reset( new Wgs84DmsParser( converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: LocationParsers destructor
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParsers::~LocationParsers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::Parse
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
bool LocationParsers::Parse( int parserId, const QString& content, geometry::Point2f& result, QStringList& hint )
{
    auto it = parsers_.find( parserId );
    if( it == parsers_.end() || it->second->GetNumberOfParameters() != 1 )
         throw MASA_EXCEPTION( "Invalid parser id or wrong number of parser parameter for this id." );
    try
    {
        return it->second->Parse( content, result, hint );
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::Parse
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
bool LocationParsers::Parse( int parserId, const QString& contentX, const QString& contentY, geometry::Point2f& result, QStringList& hint )
{
    auto it = parsers_.find( parserId );
    if( it == parsers_.end() || it->second->GetNumberOfParameters() != 2 )
        throw MASA_EXCEPTION( "Invalid parser id or wrong number of parser parameter for this id." );
    try
    {
        return it->second->Parse( contentX + ":" + contentY, result, hint );
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::GetParser
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParser_ABC& LocationParsers::GetParser( int parserId )
{
    auto it = parsers_.find( parserId );
    if( it == parsers_.end() )
        throw MASA_EXCEPTION( "Invalid parser id." );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationParsers::AddParser( LocationParser_ABC* parser, int id )
{
    if( id != kernel::CoordinateSystems::E_Mgrs && id != kernel::CoordinateSystems::E_Local &&
        id !=kernel::CoordinateSystems::E_Wgs84Dd && id != kernel::CoordinateSystems::E_Wgs84Dms )
        parsers_[ id ].reset( parser );
}
