// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_HLAIndirectFire.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_HLAIndirectFire constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
MIL_Effect_HLAIndirectFire::MIL_Effect_HLAIndirectFire( const PHY_DotationCategory& ammunition, MIL_Agent_ABC& target )
    : ammunition_( ammunition )
    , target_    ( target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_HLAIndirectFire destructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
MIL_Effect_HLAIndirectFire::~MIL_Effect_HLAIndirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_HLAIndirectFire::Execute
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
bool MIL_Effect_HLAIndirectFire::Execute()
{
    PHY_FireResults_ABC* pFireResult = new PHY_FireResults_ABC(); //$$$ BOF
    pFireResult->IncRef();
    target_.GetRole< PHY_RoleInterface_Composantes >().ApplyIndirectFire( ammunition_, *pFireResult, 1. );
    pFireResult->DecRef();
    return false;
}
