// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationPart_ABC.h"
 

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
PopulationPart_ABC::PopulationPart_ABC( uint nID, const Population& parent , E_PopulationAttitude attitude, int persons )
    : parent_       ( parent )
    , nID_          ( nID    )
    , nLivingHumans_( persons )
    , nDeadHumans_  ( 0 )
    , attitude_     ( attitude )
    , rDensity_     ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC destructor
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
PopulationPart_ABC::~PopulationPart_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetPopulation
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
const Population& PopulationPart_ABC::GetPopulation() const
{
    return parent_;
}
