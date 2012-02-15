// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_AttackPopulation.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_AttackPopulation constructor
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
MIL_Effect_AttackPopulation::MIL_Effect_AttackPopulation( MIL_PopulationElement_ABC& attacker, MIL_PopulationElement_ABC& defender, float intensity )
    : attacker_ ( attacker )
    , defender_ ( defender )
    , intensity_( intensity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_AttackPopulation destructor
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
MIL_Effect_AttackPopulation::~MIL_Effect_AttackPopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_AttackPopulation::Execute
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
bool MIL_Effect_AttackPopulation::Execute()
{
    attacker_.Attack( defender_, intensity_ );
    return false;
}
