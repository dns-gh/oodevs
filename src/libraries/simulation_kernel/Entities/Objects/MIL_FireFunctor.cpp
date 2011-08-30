// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//------------------------------------------------------------------------------
// Name: MIL_FireFunctor.cpp
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
//------------------------------------------------------------------------------

#include "simulation_kernel_pch.h"

/*
#include "MIL_FireFunctor.h"
#include "MIL_FireClass.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

// -----------------------------------------------------------------------------
// Name: MIL_FireFunctor constructor
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireFunctor::MIL_FireFunctor( const MIL_FireClass& fireClass )
    : fireClass_ ( fireClass )
    , bestExtinguisherAgentandFireHoseRange_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FireFunctor destructor
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireFunctor::~MIL_FireFunctor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator()
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
void MIL_FireFunctor::operator()( PHY_ComposantePion& composantePion )
{
    composantePion.ApplyOnWeapons( *this );
}

// -----------------------------------------------------------------------------
// Name: operator()
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
void MIL_FireFunctor::operator()( const PHY_ComposantePion& composantePion, const PHY_Weapon& weapon )
{
    MIL_FireClass::T_EvaluationResult result = fireClass_.Evaluate( weapon );
    if( result.score_ > bestExtinguisherAgentandFireHoseRange_.score_ )
    {
        bestExtinguisherAgentandFireHoseRange_.score_ = result.score_;
        bestExtinguisherAgentandFireHoseRange_.agent_ = result.agent_;
        bestExtinguisherAgentandFireHoseRange_.range_ = result.range_;
    }
}

// -----------------------------------------------------------------------------
// Name: GetExtinguisherAgentNumber
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
int MIL_FireFunctor::GetNumberOfTheExtinguisherAgent()
{
    return ( int )bestExtinguisherAgentandFireHoseRange_.agent_;
}

// -----------------------------------------------------------------------------
// Name: GetExtinguisherAgentNumber
// Created: RFT 22/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
int MIL_FireFunctor::GetFireHoseRange()
{
    return bestExtinguisherAgentandFireHoseRange_.range_;
}*/