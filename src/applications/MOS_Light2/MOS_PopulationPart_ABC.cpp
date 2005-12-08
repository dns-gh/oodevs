// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_PopulationPart_ABC.h"
 

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPart_ABC constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
MOS_PopulationPart_ABC::MOS_PopulationPart_ABC( uint nID, const MOS_Population& parent , E_PopulationAttitude attitude, int persons )
    : parent_       ( parent )
    , nID_          ( nID    )
    , nLivingHumans_( persons )
    , nDeadHumans_  ( 0 )
    , attitude_     ( attitude )
    , rDensity_     ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPart_ABC destructor
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
MOS_PopulationPart_ABC::~MOS_PopulationPart_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPart_ABC::GetPopulation
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
const MOS_Population& MOS_PopulationPart_ABC::GetPopulation() const
{
    return parent_;
}
