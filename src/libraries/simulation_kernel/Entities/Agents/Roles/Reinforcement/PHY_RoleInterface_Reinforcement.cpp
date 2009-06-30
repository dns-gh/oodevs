// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Reinforcement.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Reinforcement.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_Reinforcement, "PHY_RoleInterface_Reinforcement" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Reinforcement constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Reinforcement::PHY_RoleInterface_Reinforcement( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Reinforcement constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RoleInterface_Reinforcement::PHY_RoleInterface_Reinforcement()
    : MT_Role_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Reinforcement destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Reinforcement::~PHY_RoleInterface_Reinforcement()
{
    // NOTHING
}
