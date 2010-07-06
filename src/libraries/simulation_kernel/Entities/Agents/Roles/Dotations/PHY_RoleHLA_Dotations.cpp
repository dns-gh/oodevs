// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Dotations.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/MIL_Entity_ABC.h"

namespace dotation
{

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Dotations::PHY_RoleHLA_Dotations( MIL_Entity_ABC& entity )
: entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Dotations::~PHY_RoleHLA_Dotations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Dotations::GetConsumptionMode
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_RoleHLA_Dotations::GetConsumptionMode() const
{
    return entity_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture().GetConsumptionMode();
}

} // namespace dotation