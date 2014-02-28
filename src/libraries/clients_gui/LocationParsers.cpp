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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "ENT/ENT_Enums.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationParsers constructor
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParsers::LocationParsers( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , converter_( converter )
{
    parsers_[ eCoordinateSystem_Mgrs ].reset(
        new UtmParser( controllers_,
                       [&]( const std::string& mgrs ) { return converter.ConvertToXY( mgrs ); },
                       [&]( const geometry::Point2f& position ) { return converter.GetStringPosition( position, eCoordinateSystem_Mgrs ); } ) );
    parsers_[ eCoordinateSystem_SanC ].reset(
        new UtmParser( controllers_,
                       [&]( const std::string& s ) { return converter.ConvertFrom( s, "SAN-C" ); },
                       [&]( const geometry::Point2f& position ) { return converter.GetStringPosition( position, eCoordinateSystem_SanC ); } ) );
    parsers_[ eCoordinateSystem_Local ].reset( new XyParser( converter_ ) );
    parsers_[ eCoordinateSystem_Wgs84Dd ].reset( new Wgs84DdParser( converter_ ) );
    parsers_[ eCoordinateSystem_Wgs84Dms ].reset( new Wgs84DmsParser( converter_ ) );
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
    if( parsers_.find( id ) == parsers_.end() )
        parsers_[ id ].reset( parser );
}
