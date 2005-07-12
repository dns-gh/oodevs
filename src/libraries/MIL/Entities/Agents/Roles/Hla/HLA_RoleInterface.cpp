// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Hla/HLA_RoleInterface.cpp $
// $Author: Age $
// $Modtime: 22/11/04 16:16 $
// $Revision: 3 $
// $Workfile: HLA_RoleInterface.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_RoleInterface.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.h"

// -----------------------------------------------------------------------------
// Name: HLA_RoleInterface constructor
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
HLA_RoleInterface::HLA_RoleInterface( MT_RoleContainer& roleContainer )
    : MT_Role_ABC( roleContainer )
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
     GetRole< PHY_RoleHLA_Composantes >().Destroy();
}
