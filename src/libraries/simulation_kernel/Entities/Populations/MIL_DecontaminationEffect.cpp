// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DecontaminationEffect.h"
#include "MIL_PopulationHumans.h"

// -----------------------------------------------------------------------------
// Name: MIL_DecontaminationEffect constructor
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
MIL_DecontaminationEffect::MIL_DecontaminationEffect( MIL_PopulationHumans& humans, double delay )
    : humans_( humans )
    , delay_ ( delay )
    , tick_  ( 0u )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DecontaminationEffect destructor
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
MIL_DecontaminationEffect::~MIL_DecontaminationEffect()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DecontaminationEffect::Apply
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
bool MIL_DecontaminationEffect::Apply( double rRatioWorkers, unsigned int tick )
{
    if( rRatioWorkers > 0 && tick - tick_ >= ( delay_ / rRatioWorkers ) )
    {
        tick_ = tick;
        return humans_.ApplyDecontamination();
    }
    return false;
}
