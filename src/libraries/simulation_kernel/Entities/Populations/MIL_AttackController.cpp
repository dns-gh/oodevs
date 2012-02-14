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

// -----------------------------------------------------------------------------
// Name: MIL_AttackController constructor
// Created: LGY 2012-02-10
// -----------------------------------------------------------------------------
MIL_AttackController::MIL_AttackController( const TER_PopulationConcentrationManager& concentrationManager,
                                            MIL_EffectManager& effectManager )
    : concentrationManager_( concentrationManager )
    , effectManager_       ( effectManager )
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
void MIL_AttackController::Attack( MIL_PopulationElement_ABC& attacking, MIL_Population& defender )
{
    T_Effects updated;
    MIL_Population::T_ConcentrationVector result = defender.GetConcentration( attacking.GetLocation() );
    for( MIL_Population::CIT_ConcentrationVector it = result.begin(); it != result.end(); ++it )
    {
        if( *it && (*it)->GetID() != attacking.GetID() )
        {
            const unsigned int id = (*it)->GetID();
            T_Effect& effect = effects_[ id ];
            if( !effect )
                effect.reset( new MIL_Effect_AttackPopulation( attacking, **it ) );
            effectManager_.Register( *effect );
            updated[ id ] = effect;
        }
    }
    effects_ = updated;
}
