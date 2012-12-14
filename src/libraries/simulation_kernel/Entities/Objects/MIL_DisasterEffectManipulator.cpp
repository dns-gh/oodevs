// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DisasterEffectManipulator.h"
#include "Entities/Objects/MIL_DisasterType.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_DisasterEffectManipulator constructor
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
MIL_DisasterEffectManipulator::MIL_DisasterEffectManipulator( int thresholdId, const MIL_DisasterType& type )
    : thresholdId_( thresholdId )
    , type_       ( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterEffectManipulator destructor
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
MIL_DisasterEffectManipulator::~MIL_DisasterEffectManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterEffectManipulator::ApplyWound
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void MIL_DisasterEffectManipulator::ApplyWound( Human_ABC& human ) const
{
    human.ApplyWound( type_.GetRandomWound( thresholdId_ ) );
}
