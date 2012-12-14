// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FloodEffectManipulator.h"
#include "MIL_Random.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_FloodEffectManipulator constructor
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
MIL_FloodEffectManipulator::MIL_FloodEffectManipulator( const std::map< const PHY_HumanWound*, double >& map )
    : map_( map )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FloodEffectManipulator destructor
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
MIL_FloodEffectManipulator::~MIL_FloodEffectManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FloodEffectManipulator::ApplyWound
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void MIL_FloodEffectManipulator::ApplyWound( Human_ABC& human ) const
{
    human.ApplyWound( GetRandomWound() );
}

// -----------------------------------------------------------------------------
// Name: MIL_FloodEffectManipulator::GetRandomWound
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_FloodEffectManipulator::GetRandomWound() const
{
    const double rRand = 1. - MIL_Random::rand_ii( MIL_Random::eWounds );
    double rSumFactors = 0.;
    for( std::map< const PHY_HumanWound*, double >::const_iterator it = map_.begin(); it != map_.end(); ++it )
    {
        const PHY_HumanWound& wound = *it->first;
        rSumFactors += it->second;
        if( rSumFactors >= rRand )
            return wound;
    }
    return PHY_HumanWound::notWounded_;
}
