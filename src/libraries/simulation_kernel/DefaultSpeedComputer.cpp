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

namespace moving
{
// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
BaseSpeedComputer::BaseSpeedComputer( const SpeedStrategy_ABC& strategy )
    : strategy_          ( strategy )
    , speed_             ( std::numeric_limits<double>::max() )
    , hasUsableComponent_( false )
    , speedRatio_        ( 1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseSpeedComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
BaseSpeedComputer::~BaseSpeedComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseSpeedComputer::ApplyOnReinforcement
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void BaseSpeedComputer::ApplyOnReinforcement( const MIL_Agent_ABC& reinforcement )
{
    speed_ = std::min( speed_, strategy_.ApplyOnReinforcement( reinforcement ) );
}

// -----------------------------------------------------------------------------
// Name: BaseSpeedComputer::ApplyOnPopulation
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void BaseSpeedComputer::ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population )
{
    speed_ = std::min( speed_, strategy_.ApplyOnPopulation( population ) );
}

// -----------------------------------------------------------------------------
// Name: BaseSpeedComputer::GetSpeed
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double BaseSpeedComputer::GetSpeed() const
{
    return hasUsableComponent_ ? std::min( std::max( speedRatio_ * speed_, 5.571 ), speed_ ) : 0.;//minimum speed is set to 2km ( 5.571 pixel/deltaT ) in case where component speed is greater than 2km and speedRatio is near 0.0
}

// -----------------------------------------------------------------------------
// Name: BaseSpeedComputer::AddModifier
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void BaseSpeedComputer::AddModifier( double ratio, bool isMax )
{
    speedRatio_ *= strategy_.AddModifier( ratio, isMax );
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::DefaultSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
DefaultSpeedComputer::DefaultSpeedComputer( const SpeedStrategy_ABC& strategy )
    : BaseSpeedComputer( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputer::~DefaultSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
DefaultSpeedComputer::~DefaultSpeedComputer()
{
    // NOTHING
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
        hasUsableComponent_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: LoadedSpeedComputer::LoadedSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
LoadedSpeedComputer::LoadedSpeedComputer( const SpeedStrategy_ABC& strategy )
    : BaseSpeedComputer( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadedSpeedComputer::~LoadedSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
LoadedSpeedComputer::~LoadedSpeedComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadedSpeedComputer::ApplyOnComponent
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
void LoadedSpeedComputer::ApplyOnComponent( const PHY_ComposantePion& component )
{
    if( !component.IsLoadable() && component.IsUsable() )
    {
        speed_ = std::min( speed_, strategy_.ApplyOnComponent( component ) );
        hasUsableComponent_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: UnloadedSpeedComputer::UnloadedSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
UnloadedSpeedComputer::UnloadedSpeedComputer( const SpeedStrategy_ABC& strategy )
    : BaseSpeedComputer( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnloadedSpeedComputer::~UnloadedSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
UnloadedSpeedComputer::~UnloadedSpeedComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnloadedSpeedComputer::ApplyOnComponent
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
void UnloadedSpeedComputer::ApplyOnComponent( const PHY_ComposantePion& component )
{
    if( component.IsLoadable() && component.IsUsable() )
    {
        speed_ = std::min( speed_, strategy_.ApplyOnComponent( component ) );
        hasUsableComponent_ = true;
    }
}

}
