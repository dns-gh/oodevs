// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_KillOfficers.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:33 $
// $Revision: 4 $
// $Workfile: MIL_Effect_KillOfficers.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_KillOfficers.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_KillOfficers constructor
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
MIL_Effect_KillOfficers::MIL_Effect_KillOfficers( MIL_Agent_ABC& target )
    : target_( target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_KillOfficers destructor
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
MIL_Effect_KillOfficers::~MIL_Effect_KillOfficers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_KillOfficers::Execute
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
bool MIL_Effect_KillOfficers::Execute()
{
    target_.GetRole< human::PHY_RoleInterface_Humans >().ChangeHumansAvailability( PHY_HumanRank::officier_, 0 );
    delete this;
    return false; // Effect must be stopped
}
