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

Unit Units::meters( "m" );
Unit Units::degrees( "°" );
Unit Units::percentage( "%" );
Unit Units::metersPerSecond( "m/s" );