// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RoleHLA_Dotations.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 16:40 $
// $Revision: 3 $
// $Workfile: PHY_RoleHLA_Dotations.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Dotations.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Dotations::PHY_RoleHLA_Dotations( MT_RoleContainer& roleContainer )
    : PHY_RoleInterface_Dotations( roleContainer )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Dotations::~PHY_RoleHLA_Dotations()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations::GetConsumptionMode
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_RoleHLA_Dotations::GetConsumptionMode() const
{
    return GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture().GetConsumptionMode();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations::HasDotation
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Dotations::HasDotation( const PHY_DotationCategory& /*category*/ ) const
{
    return true;
}
