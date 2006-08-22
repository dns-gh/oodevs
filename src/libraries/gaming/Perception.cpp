// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Perception.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Perception constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Perception::Perception( const Agent_ABC& detected, E_PerceptionResult level )
    : detected_( &detected )
    , level_( level )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Perception destructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Perception::~Perception()
{
    // NOTHING
}
