// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 14:52 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Communications.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RoleInterface_Communications.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Communications constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Communications::PHY_RoleInterface_Communications( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Communications constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleInterface_Communications::PHY_RoleInterface_Communications()
    : MT_Role_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Communications destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Communications::~PHY_RoleInterface_Communications()
{
    // NOTHING
}
