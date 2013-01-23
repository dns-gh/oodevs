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
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect constructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
MIL_ContaminationEffect::MIL_ContaminationEffect( MIL_PopulationHumans& humans, double delay, unsigned int time )
    : delay_ ( delay )
    , humans_( &humans )
    , time_  ( time )
{
    humans_->ApplyContamination();
}

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect constructor
// Created: JSR 2012-03-07
// -----------------------------------------------------------------------------
MIL_ContaminationEffect::MIL_ContaminationEffect()
    : delay_ ( 0 )
    , humans_( 0 )
    , time_  ( 0 )
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
        humans_->ApplyContamination();
        time_ = time;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect::load
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
void MIL_ContaminationEffect::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> delay_;
    ar >> humans_;
    ar >> time_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ContaminationEffect::save
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
void MIL_ContaminationEffect::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << delay_;
    ar << humans_;
    ar << time_;
}
