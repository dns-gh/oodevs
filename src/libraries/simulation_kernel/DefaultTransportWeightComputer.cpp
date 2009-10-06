// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DefaultTransportWeightComputer.h"

namespace transport
{

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportWeightComputer::DefaultTransportWeightComputer() :
  totalTransportedWeight_(0.),
  heaviestTransportedWeight_(0.),
  strategy_(0)
{

}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportWeightComputer::~DefaultTransportWeightComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::Reset
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportWeightComputer::Reset(const TransportStrategy_ABC* st)
{
  totalTransportedWeight_ = heaviestTransportedWeight_ = 0.;
  strategy_ = st;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::AddTransportedWeight
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportWeightComputer::AddTransportedWeight(double weight, bool canBeLoaded)
{
    if(!strategy_ || (strategy_ && strategy_->Autorize(canBeLoaded) ) )
    {
        totalTransportedWeight_ += weight;
        heaviestTransportedWeight_ = std::max(heaviestTransportedWeight_, weight);
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::TotalTransportedWeight
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportWeightComputer::TotalTransportedWeight() const
{
  return totalTransportedWeight_;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::HeaviestTransportedWeight
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportWeightComputer::HeaviestTransportedWeight() const
{
 return  heaviestTransportedWeight_;
}

}
