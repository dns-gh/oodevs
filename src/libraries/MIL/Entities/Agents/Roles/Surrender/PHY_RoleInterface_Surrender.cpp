// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 15:09 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Surrender.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoleInterface_Surrender.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Surrender constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Surrender::PHY_RoleInterface_Surrender( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Surrender constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RoleInterface_Surrender::PHY_RoleInterface_Surrender()
    : MT_Role_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Surrender::~PHY_RoleInterface_Surrender()
{
}
