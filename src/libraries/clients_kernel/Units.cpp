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
Unit::Unit( const QString& symbol )
    : symbol_( symbol )
{
    // NOTHING
}

const Unit Units::meters            ( tools::translate( "Units", "m" ) );
const Unit Units::degrees           ( tools::translate( "Units", "°" ) );
const Unit Units::percentage        ( tools::translate( "Units", "%" ) );
const Unit Units::metersPerSecond   ( tools::translate( "Units", "m/s" ) );
const Unit Units::vehiclesPerHour   ( tools::translate( "Units", "veh/h" ) );
const Unit Units::tons              ( tools::translate( "Units", "t" ) );
const Unit Units::kilometersPerHour ( tools::translate( "Units", "km/h" ) );
const Unit Units::hours             ( tools::translate( "Units", "h" ) );

