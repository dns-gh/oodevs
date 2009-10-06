// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultHumanLoadingTimeComputer.h"

namespace transport
{

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultHumanLoadingTimeComputer::DefaultHumanLoadingTimeComputer() :
    humansCount_(0),
    humansLoadedPerTimeStep_(0.),
    humansUnloadedPerTimeStep_(0.)
{

}

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultHumanLoadingTimeComputer::~DefaultHumanLoadingTimeComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer::AddHumans
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultHumanLoadingTimeComputer::AddHumans(int humansCount)
{
    humansCount_ += humansCount;
}

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer::AddTime
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultHumanLoadingTimeComputer::AddTime(double loadingPerTimeStep, double unloadingPerTimeStep)
{
    humansLoadedPerTimeStep_+=loadingPerTimeStep;
    humansUnloadedPerTimeStep_+=unloadingPerTimeStep;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest::Reset(
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultHumanLoadingTimeComputer::Reset()
{
    humansCount_ = 0;
    humansLoadedPerTimeStep_ = humansUnloadedPerTimeStep_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer::GetHumansCount
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
int DefaultHumanLoadingTimeComputer::GetHumansCount() const
{
    return humansCount_;
}

// -----------------------------------------------------------------------------
// Name: DefaultHumanLoadingTimeComputer::GetHumansLoadedPerTimeStep
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultHumanLoadingTimeComputer::GetHumansLoadedPerTimeStep() const
{
    return humansLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest::GetHumansUnloadedPerTimeStep
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultHumanLoadingTimeComputer::GetHumansUnloadedPerTimeStep() const
{
    return humansUnloadedPerTimeStep_;
}
}
