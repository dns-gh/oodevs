// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Units.h"

// -----------------------------------------------------------------------------
// Name: Unit::Unit
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Unit::Unit( const QString& symbol )
    : symbol_( symbol )
{
    // NOTHING
}

const Unit Units::meters( "m" );
const Unit Units::degrees( "°" );
const Unit Units::percentage( "%" );
const Unit Units::metersPerSecond( "m/s" );
const Unit Units::vehiclesPerHour( "veh/h" );
const Unit Units::tons( "t" );
