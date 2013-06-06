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

#include "simulation_kernel_pch.h"
#include "MIL_Effect_PopulationFire.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Orders/MIL_Report.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_PopulationFire::MIL_Effect_PopulationFire( MIL_Population& population, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, double armedIndividuals )
    : population_        ( population )
    , populationType_    ( populationType )
    , populationAttitude_( populationAttitude )
    , target_            ( target )
    , compTarget_        ( compTarget )
    , fireResult_        ( fireResult )
    , armedIndividuals_  ( armedIndividuals )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_PopulationFire::~MIL_Effect_PopulationFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_PopulationFire::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_PopulationFire::Execute()
{
    bool bCanBePlundered = false;
    target_.GetRole< PHY_RoleInterface_Composantes >().ApplyPopulationFire( compTarget_, populationType_, populationAttitude_, fireResult_, armedIndividuals_, bCanBePlundered );
    if( bCanBePlundered )
    {
        population_.AddArmedHumans( 1 );
        MIL_Report::PostEvent( target_, MIL_Report::eRC_MilitaryEquipmentPlundered );
    }
    delete this;
    return false; // Effect must be stopped
}
