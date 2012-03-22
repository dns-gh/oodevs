// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
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
    if( attacking.GetHealthyHumans() > 0 )
    {
        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( attacking.GetLocation(), concentrations );
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
        {
            MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *it );
            const unsigned int attackerId = attacking.GetID();
            if( pElement )
            {
                const unsigned int id = pElement->GetID();
                if( id != attackerId && pElement->GetHealthyHumans() > 0 )
                {
                    T_Effect& effect = effects_[ id ];
                    if( !effect )
                        effect.reset( new MIL_Effect_AttackPopulation( attacking, *pElement ) );
                    MIL_EffectManager::GetEffectManager().Register( *effect );
                    updated[ id ] = effect;
                }
            }
        }
    }
    effects_ = updated;
}
