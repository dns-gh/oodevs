// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_RoleInterface.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: HLA_RoleInterface constructor
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
HLA_RoleInterface::HLA_RoleInterface( MIL_Agent_ABC& agent )
    : pAgent_( &agent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleInterface destructor
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
HLA_RoleInterface::~HLA_RoleInterface()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleInterface::Destroy
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void HLA_RoleInterface::Destroy()
{
    assert( pAgent_ );
    pAgent_->GetRole< PHY_RoleHLA_Composantes >().Destroy();
}
