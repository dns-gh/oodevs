// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultUrbanLocationComputer.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include <urban/TerrainObject_ABC.h>

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::DefaultUrbanLocationComputer
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
DefaultUrbanLocationComputer::DefaultUrbanLocationComputer( const urban::TerrainObject_ABC& block, float urbanDeployment )
    : block_          ( block )
    , urbanDeployment_( urbanDeployment )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::~DefaultUrbanLocationComputer
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
DefaultUrbanLocationComputer::~DefaultUrbanLocationComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::ComputeUrbanDeployment
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void DefaultUrbanLocationComputer::ComputeUrbanDeployment( const PHY_Posture& currentPosture )
{
    if( &currentPosture == &PHY_Posture::poste_ || &currentPosture == &PHY_Posture::posteAmenage_ || &currentPosture == &PHY_Posture::postePrepareGenie_  )
    {
        urbanDeployment_ = urbanDeployment_ + ( 100 + urbanDeployment_ )/( block_.ComputeComplexity() );
        if( urbanDeployment_ > 1.f )
            urbanDeployment_ = 1.f;
    }
        
    else
        urbanDeployment_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::Result
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
float DefaultUrbanLocationComputer::Result()
{
    return urbanDeployment_;
}