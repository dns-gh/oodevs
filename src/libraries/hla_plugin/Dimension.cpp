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

// -----------------------------------------------------------------------------
// Name: Dimension constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Dimension::Dimension( float x, float y, float z )
    : xAxisLength_( x )
    , yAxisLength_( y )
    , zAxisLength_( z )
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
