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
DefaultTransportWeightComputer::DefaultTransportWeightComputer( const TransportStrategy_ABC* strategy )
    : totalTransportedWeight_   ( 0. )
    , heaviestTransportedWeight_( 0. )
    , strategy_                 ( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportWeightComputer::~DefaultTransportWeightComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::AddTransportedWeight
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportWeightComputer::AddTransportedWeight(double weight, bool canBeLoaded)
{
    if( !strategy_ || (strategy_ && strategy_->Authorize( canBeLoaded ) ) )
    {
        totalTransportedWeight_ += weight;
        heaviestTransportedWeight_ = std::max( heaviestTransportedWeight_, weight );
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

// -----------------------------------------------------------------------------
// Name: DefaultTransportWeightComputer::CanTransportDestroyed
// Created: JSR 2014-02-07
// -----------------------------------------------------------------------------
bool DefaultTransportWeightComputer::CanTransportDestroyed() const
{
    return strategy_ ? strategy_->CanTransportDestroyed() : false;
}

}
