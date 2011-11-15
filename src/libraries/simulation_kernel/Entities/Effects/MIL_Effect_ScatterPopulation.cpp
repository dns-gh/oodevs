// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_ScatterPopulation.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ScatterPopulation constructor
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
MIL_Effect_ScatterPopulation::MIL_Effect_ScatterPopulation( MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult )
    : target_       ( target )
    , nNbrAmmoFired_( nNbrAmmoFired )
    , fireResult_   ( fireResult )
{
    fireResult_.IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ScatterPopulation destructor
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
MIL_Effect_ScatterPopulation::~MIL_Effect_ScatterPopulation()
{
    fireResult_.DecRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_ScatterPopulation::Execute
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
bool MIL_Effect_ScatterPopulation::Execute()
{
    target_.ApplyFire( nNbrAmmoFired_, fireResult_, false );
    delete this;
    return false; // Effect must be stopped
}
