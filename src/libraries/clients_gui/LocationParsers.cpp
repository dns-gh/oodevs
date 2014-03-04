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
{
    parsers_[ eCoordinateSystem_Mgrs ].reset(
        new UtmParser( controllers,
                       [&]( const std::string& mgrs ) { return converter.ConvertToXY( mgrs ); },
                       [&]( const geometry::Point2f& position ) { return converter.GetStringPosition( position, eCoordinateSystem_Mgrs ); } ) );
    parsers_[ eCoordinateSystem_SanC ].reset(
        new UtmParser( controllers,
                       [&]( const std::string& s ) { return converter.ConvertFrom( s, "SAN-C" ); },
                       [&]( const geometry::Point2f& position ) { return converter.GetStringPosition( position, eCoordinateSystem_SanC ); } ) );
    parsers_[ eCoordinateSystem_Local ].reset( new XyParser( converter ) );
    parsers_[ eCoordinateSystem_Wgs84Dd ].reset( new Wgs84DdParser( converter ) );
    parsers_[ eCoordinateSystem_Wgs84Dms ].reset( new Wgs84DmsParser( converter ) );
}

// -----------------------------------------------------------------------------
// Name: LocationParsers destructor
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
LocationParsers::~LocationParsers()
{
    parsers_.clear();
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::GetParser
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
const std::shared_ptr< const LocationParser_ABC >& LocationParsers::GetParser( int parserId ) const
{
    auto it = parsers_.find( parserId );
    if( it == parsers_.end() )
        throw MASA_EXCEPTION( "Invalid parser id." );
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: LocationParsers::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationParsers::AddParser( const std::shared_ptr< const LocationParser_ABC >& parser, int id )
{
    if( parsers_.find( id ) != parsers_.end() )
        throw MASA_EXCEPTION( "Parser id already present." );
    parsers_[ id ] = parser;
}
