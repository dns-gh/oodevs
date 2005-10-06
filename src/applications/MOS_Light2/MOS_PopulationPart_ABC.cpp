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
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
MOS_PopulationPart_ABC::MOS_PopulationPart_ABC( uint nID, const MOS_Population& parent )
    : parent_       ( parent )
    , nID_          ( nID    )
    , nLivingHumans_( 0 )
    , nDeadHumans_  ( 0 )
    , attitude_     ( ePopulationAttitude_Calme )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPart_ABC destructor
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
MOS_PopulationPart_ABC::~MOS_PopulationPart_ABC()
{
}

