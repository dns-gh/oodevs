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
Unit::Unit( const QString& symbol, double minValue, double maxValue )
    : symbol_( symbol )
    , minValue_( minValue )
    , maxValue_( maxValue )
{
    // NOTHING
}

const Unit Units::meters            ( tools::translate( "Units", "m" )      , 0., std::numeric_limits< int >::max() );
const Unit Units::degrees           ( tools::translate( "Units", "°" )      , 0., 360. );
const Unit Units::percentage        ( tools::translate( "Units", "%" )      , 0., 100. );
const Unit Units::percentageGTzero  ( tools::translate( "Units", "%" )      , 1., 100. );
const Unit Units::metersPerSecond   ( tools::translate( "Units", "m/s" )    , 0., std::numeric_limits< int >::max() );
const Unit Units::vehiclesPerHour   ( tools::translate( "Units", "veh/h" )  , 0., std::numeric_limits< int >::max() );
const Unit Units::tons              ( tools::translate( "Units", "t" )      , 0., std::numeric_limits< int >::max() );
const Unit Units::kilometersPerHour ( tools::translate( "Units", "km/h" )   , 0., std::numeric_limits< int >::max() );
const Unit Units::hours             ( tools::translate( "Units", "h" )      , 0., std::numeric_limits< int >::max() );
const Unit Units::minesPerMeter     ( tools::translate( "Units", "mines/m" ), 0., std::numeric_limits< int >::max() );

