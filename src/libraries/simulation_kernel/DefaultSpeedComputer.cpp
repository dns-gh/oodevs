// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DefaultSpeedComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultSpeedComputer::DefaultSpeedComputer() :
        strategy_          ( 0 ),
        speed_             ( std::numeric_limits<double>::max() ),
        hasUsableComponent_( false ),
        speedRatio_        ( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultSpeedComputer::~DefaultSpeedComputer()
{
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::Reset
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::Reset( const SpeedStrategy_ABC* strategy )
{
    strategy_ = strategy;
    speed_ = std::numeric_limits<double>::max();
    hasUsableComponent_ = false;
    speedRatio_ = 1.;
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnComponent
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnComponent( const PHY_ComposantePion& component )
{
    assert( strategy_ );
    if( component.CanMove() )
    {
        speed_ = std::min( speed_, strategy_->ApplyOnComponent( component ) );
        hasUsableComponent_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnReinforcement
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnReinforcement( MIL_AgentPion& reinforcement)
{
    assert( strategy_ );
    speed_ = std::min( speed_, strategy_->ApplyOnReinforcement( reinforcement ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnPopulation
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population)
{
    assert( strategy_ );
    speed_ = std::min( speed_, strategy_->ApplyOnPopulation( population ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::GetSpeed
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultSpeedComputer::GetSpeed() const
{
    return hasUsableComponent_ ? speedRatio_ * speed_ : 0.;
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::AddModifier
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::AddModifier( double ratio, bool isMax )
{
    speedRatio_ *= strategy_->AddModifier( ratio, isMax );
}

}
