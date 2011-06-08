// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Dimension.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Dimension constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Dimension::Dimension( bool isOnRoad )
    : xAxisLength_( isOnRoad ? 200.f : 100.f )
    , yAxisLength_( isOnRoad ?  50.f : 100.f )
    , zAxisLength_( 3.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dimension destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Dimension::~Dimension()
{
    // NOTHING
}
