// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_Suicide.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Suicide constructor
// Created: LDC 2010-03-29
// -----------------------------------------------------------------------------
MIL_Effect_Suicide::MIL_Effect_Suicide( MIL_Agent_ABC& target )
: target_( target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Suicide destructor
// Created: LDC 2010-03-29
// -----------------------------------------------------------------------------
MIL_Effect_Suicide::~MIL_Effect_Suicide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Suicide::Execute
// Created: LDC 2010-03-29
// -----------------------------------------------------------------------------
bool MIL_Effect_Suicide::Execute()
{
    human::PHY_RoleInterface_Humans& humanRole = target_.GetRole< human::PHY_RoleInterface_Humans >();
    const auto& ranks = PHY_HumanRank::GetHumanRanks();
    for( auto it = ranks.begin(); it != ranks.end(); ++it )
        humanRole.ChangeHumansAvailability( *it->second, 0 );

    PHY_RolePion_Composantes& roleComposantes = target_.GetRole< PHY_RolePion_Composantes >();
    roleComposantes.DestroyAllComposantes();

    delete this;
    return false;
}
