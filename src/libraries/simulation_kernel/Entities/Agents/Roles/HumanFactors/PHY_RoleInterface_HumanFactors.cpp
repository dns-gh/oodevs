// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 14:06 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_HumanFactors.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_HumanFactors.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_HumanFactors, "PHY_RoleInterface_HumanFactors" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_HumanFactors constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_HumanFactors::PHY_RoleInterface_HumanFactors( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_HumanFactors constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleInterface_HumanFactors::PHY_RoleInterface_HumanFactors()
    : MT_Role_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_HumanFactors destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_HumanFactors::~PHY_RoleInterface_HumanFactors()
{
    // NOTHING
}
