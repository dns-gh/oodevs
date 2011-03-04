// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_BurnEffectManipulator.h"
#include "MIL_FireClass.h"

// -----------------------------------------------------------------------------
// Name: MIL_BurnEffectManipulator constructor
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
MIL_BurnEffectManipulator::MIL_BurnEffectManipulator( const MIL_FireClass& fireClass )
: pFireClass_( &fireClass )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_BurnEffectManipulator::GetRandomWound
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* MIL_BurnEffectManipulator::ChooseRandomWound() const
{
    return pFireClass_->ChooseRandomWound();
}
