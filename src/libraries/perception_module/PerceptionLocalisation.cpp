// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "PerceptionLocalisation.h"
#include "wrapper/Hook.h"

using namespace sword::perception;

DECLARE_HOOK( GetPerceptionId, int, () )

// -----------------------------------------------------------------------------
// Name: PerceptionLocalisation constructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
PerceptionLocalisation::PerceptionLocalisation()
    : id_( GET_HOOK( GetPerceptionId )() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionLocalisation destructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
PerceptionLocalisation::~PerceptionLocalisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionLocalisation::Id
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
int PerceptionLocalisation::Id() const
{
    return id_;
}
