// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultPostureComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"

using namespace posture;

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::DefaultPostureComputer()
: bIsLoaded_( false )
, bDiscreteModeEnabled_( false )
, bMustBeForce_( false )
, pCurrentPosture_( 0 )
, pNewPosture_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::~DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::~DefaultPostureComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::~DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::Reset( const PHY_Posture& posture, bool bIsLoaded, bool bDiscreteModeEnabled )
{
    pCurrentPosture_ = &posture;
    pNewPosture_ = 0;
    bIsLoaded_ = bIsLoaded;
    bDiscreteModeEnabled_ = bDiscreteModeEnabled;
    bMustBeForce_ = false;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::SetPostureMovement
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::SetPostureMovement()
{
    if( !bIsLoaded_ )
        pNewPosture_ = &PHY_Posture::posteReflexe_;
    else if( bDiscreteModeEnabled_ )
        pNewPosture_ = &PHY_Posture::mouvementDiscret_;
    else
        pNewPosture_ = &PHY_Posture::mouvement_;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::UnsetPostureMovement
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::UnsetPostureMovement()
{
    if ( pCurrentPosture_ == &PHY_Posture::mouvement_ 
        || pCurrentPosture_ == &PHY_Posture::mouvementDiscret_ )
        pNewPosture_ = &PHY_Posture::arret_;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::MustBeForce
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
bool DefaultPostureComputer::MustBeForce()
{
    return bMustBeForce_;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::GetPosture
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
const PHY_Posture& DefaultPostureComputer::GetPosture()
{
    return *pNewPosture_;
}