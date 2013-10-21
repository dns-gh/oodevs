// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Posture.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"

bool IsPosted( const MIL_Agent_ABC& agent )
{
    const auto& posture = agent.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    return &posture == &PHY_Posture::poste_ || &posture == &PHY_Posture::posteAmenage_;
}
