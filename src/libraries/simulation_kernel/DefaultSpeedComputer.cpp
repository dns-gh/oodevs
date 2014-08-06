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
#include "Entities/Agents/Actions/Moving/SpeedStrategy_ABC.h"

using namespace moving;

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultSpeedComputer::DefaultSpeedComputer( const SpeedStrategy_ABC& strategy )
    : strategy_    ( strategy )
    , speed_       ( std::numeric_limits< double >::max() )
    , hasComponent_( false )
    , speedRatio_  ( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultSpeedComputer::~DefaultSpeedComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnReinforcement
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnReinforcement( const MIL_Agent_ABC& reinforcement )
{
    speed_ = std::min( speed_, strategy_.ApplyOnReinforcement( reinforcement ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnPopulation
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population )
{
    speed_ = std::min( speed_, strategy_.ApplyOnPopulation( population ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::GetSpeed
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultSpeedComputer::GetSpeed() const
{
    if( ! hasComponent_ )
        return 0;
    // minimum speed is set to 2km ( 5.571 pixel/deltaT ) in case where component speed is greater than 2km and speedRatio is near 0.0
    return std::min( std::max( speedRatio_ * speed_, 5.571 ), speed_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::AddModifier
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::AddModifier( double ratio, bool isMax )
{
    speedRatio_ *= strategy_.AddModifier( ratio, isMax );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::ApplyOnComponent
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultSpeedComputer::ApplyOnComponent( const PHY_ComposantePion& component )
{
    if( component.CanMove() )
    {
        speed_ = std::min( speed_, strategy_.ApplyOnComponent( component ) );
        hasComponent_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::IsTheoric
// Created: LDC 2014-08-05
// -----------------------------------------------------------------------------
bool DefaultSpeedComputer::IsTheoric() const
{
    return strategy_.IsTheoric();
}
