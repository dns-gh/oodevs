// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AttackController.h"
#include "Entities/Effects/MIL_Effect_AttackPopulation.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Populations/MIL_Population.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: MIL_AttackController constructor
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
MIL_AttackController::MIL_AttackController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AttackController destructor
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
MIL_AttackController::~MIL_AttackController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AttackController::Attack
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
void MIL_AttackController::Attack( MIL_PopulationElement_ABC& attacking )
{
    T_Effects updated;
    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( attacking.GetLocation(), concentrations, 10 );
    for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
    {
        MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *it );
        if( pElement && pElement->GetID() != attacking.GetID() && pElement->GetHealthyHumans() > 0 && attacking.GetHealthyHumans() > 0 )
        {
            const unsigned int id = pElement->GetID();
            T_Effect& effect = effects_[ id ];
            if( !effect )
                effect.reset( new MIL_Effect_AttackPopulation( attacking, *pElement ) );
            MIL_EffectManager::GetEffectManager().Register( *effect );
            updated[ id ] = effect;
        }
    }
    effects_ = updated;
}