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

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::DefaultUrbanLocationComputer
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
DefaultUrbanLocationComputer::DefaultUrbanLocationComputer()
    : results_()
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
// Name: DefaultUrbanLocationComputer::SetUrbanDeployment
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void DefaultUrbanLocationComputer::SetUrbanDeployment( double urbanDeployment )
{
    results_.urbanDeployment_ = urbanDeployment;
}

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::SetPosition
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void DefaultUrbanLocationComputer::SetPosition( const MT_Vector2D& position )
{
    results_.position_ = position;
}

// -----------------------------------------------------------------------------
// Name: DefaultUrbanLocationComputer::GetPosture
// Created: SLG 2010-04-20
// -----------------------------------------------------------------------------
UrbanLocationComputer_ABC::Results& DefaultUrbanLocationComputer::Result()
{
    return results_;
}
