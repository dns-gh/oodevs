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
#include "Tools.h"
#include "UtmParser.h"
#include "XYParser.h"
#include "Wgs84DdParser.h"
#include "Wgs84DmsParser.h"
#include "FeatureNameParser.h"
#include "clients_kernel/CoordinateSystems.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationParsers constructor
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParsers::LocationParsers( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , converter_  ( converter )
{
    FillParsers();
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::FillParsers
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationParsers::FillParsers()
{
    parsers_[ kernel::CoordinateSystems::E_Mgrs ] = new UtmParser( controllers_, converter_ );
    parsers_[ kernel::CoordinateSystems::E_Local ] = new XyParser();
    parsers_[ kernel::CoordinateSystems::E_Wgs84Dd ] = new Wgs84DdParser( converter_ );
    parsers_[ kernel::CoordinateSystems::E_Wgs84Dms ] = new Wgs84DmsParser( converter_ );
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
bool LocationParsers::Parse( int parserId, QString content, geometry::Point2f& result, QStringList& hint )
{
    if( parsers_.find( parserId ) == parsers_.end() || parsers_[ parserId ]->GetNumberOfParameters() != 1 )
         throw MASA_EXCEPTION( "Invalid parser id or wrong number of parser parameter for this id." );
    try
    {
        return parsers_[ parserId ]->Parse( content, result, hint );
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
bool LocationParsers::Parse( int parserId, QString contentX, QString contentY, geometry::Point2f& result, QStringList& hint )
{
    if( parsers_.find( parserId ) == parsers_.end() || parsers_[ parserId ]->GetNumberOfParameters() != 2 )
        throw MASA_EXCEPTION( "Invalid parser id or wrong number of parser parameter for this id." );
    try
    {
        return parsers_[ parserId ]->Parse( contentX + ":" + contentY, result, hint );
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
    if( parsers_.find( parserId ) == parsers_.end() )
        throw MASA_EXCEPTION( "Invalid parser id." );
    return **parsers_.find( parserId )->second;
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationParsers::AddParser( LocationParser_ABC& parser, int id )
{
    parsers_[ id ] = &parser;
}
