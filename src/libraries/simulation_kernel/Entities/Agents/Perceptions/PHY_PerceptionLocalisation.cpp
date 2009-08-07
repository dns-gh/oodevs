// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHy_PerceptionLocalisation.h"
#include "PHY_Perception_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLocalisation constructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
PHY_PerceptionLocalisation::PHY_PerceptionLocalisation()
: id_( PHY_Perception_ABC::GetPerceptionId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLocalisation destructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
PHY_PerceptionLocalisation::~PHY_PerceptionLocalisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLocalisation::Id
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
int PHY_PerceptionLocalisation::Id() const
{
    return id_;
}
