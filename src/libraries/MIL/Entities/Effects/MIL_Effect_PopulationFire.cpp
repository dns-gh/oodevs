// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_PopulationFire.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:33 $
// $Revision: 4 $
// $Workfile: MIL_Effect_PopulationFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_Effect_PopulationFire.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_PopulationFire::MIL_Effect_PopulationFire( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult )
    : populationType_    ( populationType )
    , populationAttitude_( populationAttitude )
    , target_            ( target )
    , compTarget_        ( compTarget )
    , fireResult_        ( fireResult )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_PopulationFire::~MIL_Effect_PopulationFire()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_PopulationFire::Execute()
{
    target_.GetRole< PHY_RoleInterface_Composantes >().ApplyPopulationFire( compTarget_, populationType_, populationAttitude_, fireResult_ );
    delete this;
    return false; // Effect must be stopped
}
