// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RoleInterface_Maintenance.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Maintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Maintenance.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_Maintenance, "PHY_RoleInterface_Maintenance" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance::PHY_RoleInterface_Maintenance( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Maintenance constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance::PHY_RoleInterface_Maintenance()
    : MT_Role_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance::~PHY_RoleInterface_Maintenance()
{
    // NOTHING
}
