// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Units.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Unit::Unit
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Unit::Unit( const QString& symbol, const QString& /*dummy*/, double minValue, double maxValue, unsigned short decimal )
    : symbol_  ( symbol )
    , minValue_( minValue )
    , maxValue_( maxValue )
    , decimal_ ( decimal )
{
    // NOTHING
}

const Unit Units::meters            ( "m",       tools::translate( "Units", "m" )      , 0., std::numeric_limits< int >::max(), 2 );
const Unit Units::degrees           ( "°",       tools::translate( "Units", "°" )      , 0., 360.                             , 2 );
const Unit Units::percentage        ( "%",       tools::translate( "Units", "%" )      , 0., 100.                             , 2 );
const Unit Units::percentageGTzero  ( "%",       tools::translate( "Units", "%" )      , 1., 100.                             , 2 );
const Unit Units::metersPerSecond   ( "m/s",     tools::translate( "Units", "m/s" )    , 0., std::numeric_limits< int >::max(), 2 );
const Unit Units::vehiclesPerHour   ( "veh/h",   tools::translate( "Units", "veh/h" )  , 0., std::numeric_limits< int >::max(), 2 );
const Unit Units::tons              ( "t",       tools::translate( "Units", "t" )      , 0., std::numeric_limits< int >::max(), 3 );
const Unit Units::kilometersPerHour ( "km/h",    tools::translate( "Units", "km/h" )   , 0., std::numeric_limits< int >::max(), 2 );
const Unit Units::hours             ( "h",       tools::translate( "Units", "h" )      , 0., std::numeric_limits< int >::max(), 2 );
const Unit Units::minesPerMeter     ( "mines/m", tools::translate( "Units", "mines/m" ), 0., std::numeric_limits< int >::max(), 2 );

