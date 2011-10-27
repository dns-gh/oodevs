// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DefaultTransportCapacityComputer.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace transport {

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportCapacityComputer::DefaultTransportCapacityComputer()
    : rWeightCapacity_                ( 0. )
    , rWeightLoadedPerTimeStep_       ( 0. )
    , rWeightUnloadedPerTimeStep_     ( 0. )
    , rMaxComposanteTransportedWeight_( 0. )
{
    // TODO Auto-generated constructor stub

}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportCapacityComputer::~DefaultTransportCapacityComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer::ApplyOnComposante
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportCapacityComputer::ApplyOnComposante( PHY_ComposantePion& composante )
{
    if( composante.CanTransportPion() )
    {
        rWeightCapacity_                 += composante.GetPionTransporterWeightCapacity           ();
        rWeightLoadedPerTimeStep_        += composante.GetPionTransporterWeightLoadedPerTimeStep  ();
        rWeightUnloadedPerTimeStep_      += composante.GetPionTransporterWeightUnloadedPerTimeStep();
        rMaxComposanteTransportedWeight_  = std::max( rMaxComposanteTransportedWeight_, composante.GetPionTransporterWeightCapacity() );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer::WeightCapacity
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportCapacityComputer::WeightCapacity() const
{
    return rWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer::WeightLoadedPerTimeStep
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportCapacityComputer::WeightLoadedPerTimeStep() const
{
    return rWeightLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer::WeightUnloadedPerTimeStep
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportCapacityComputer::WeightUnloadedPerTimeStep() const
{
    return rWeightUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportCapacityComputer::MaxComposanteTransportedWeight
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultTransportCapacityComputer::MaxComposanteTransportedWeight() const
{
    return rMaxComposanteTransportedWeight_;
}
}
