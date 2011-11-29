// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ContaminationEffect.h"
#include "MIL_PopulationHumans.h"

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect constructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
MIL_ContaminationEffect::MIL_ContaminationEffect( MIL_PopulationHumans& humans, double delay, unsigned int time )
    : delay_ ( delay )
    , humans_( humans )
    , time_  ( time )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect destructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
MIL_ContaminationEffect::~MIL_ContaminationEffect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect::Update
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
void MIL_ContaminationEffect::Update( unsigned int time )
{
    if( time - time_ >= delay_ )
    {
        humans_.ApplyContamination();
        time_ = time;
    }
}
