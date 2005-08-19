// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 12:16 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Location.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoleInterface_Location.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Location constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Location::PHY_RoleInterface_Location( MT_RoleContainer& role )
    : MT_Role_ABC  ( role )
    , TER_Agent_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Location constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RoleInterface_Location::PHY_RoleInterface_Location()
    : MT_Role_ABC  ()
    , TER_Agent_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Location destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Location::~PHY_RoleInterface_Location()
{
}

