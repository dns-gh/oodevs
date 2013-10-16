// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AutomatManipulator.h"
#include "dispatcher/Automat.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: AutomatManipulator constructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatManipulator::AutomatManipulator( const dispatcher::Automat& automat )
    : automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatManipulator destructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatManipulator::~AutomatManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatManipulator::RegisterIn
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
void AutomatManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier", &AutomatManipulator::GetIdentifier );
}

// -----------------------------------------------------------------------------
// Name: AutomatManipulator::GetIdentifier
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
unsigned int AutomatManipulator::GetIdentifier() const
{
    return automat_.GetId();
}
