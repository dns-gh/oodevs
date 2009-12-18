// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultPerceptionDistanceComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace detection
{

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::DefaultPerceptionDistanceComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultPerceptionDistanceComputer::DefaultPerceptionDistanceComputer()
    : elongation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::~DefaultPerceptionDistanceComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultPerceptionDistanceComputer::~DefaultPerceptionDistanceComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::AddModifier
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void DefaultPerceptionDistanceComputer::AddModifier( double modifier )
{
    if( modifier != 0. )
        modifiers_.push_back( modifier );
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::AddModifier
// Created: MGD 2009-10-16
// -----------------------------------------------------------------------------
void DefaultPerceptionDistanceComputer::AddElongationFactor( double modifier )
{
    elongation_ = modifier;
    AddModifier( modifier );
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::GetDuration
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
double DefaultPerceptionDistanceComputer::GetFactor() const
{
    double factor = 1.0;
    for( std::vector< double >::const_iterator it = modifiers_.begin(); it != modifiers_.end(); it++ )
    {
        factor *= *it;
    }
    return factor;
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::AddModifier
// Created: MGD 2009-10-16
// -----------------------------------------------------------------------------
double DefaultPerceptionDistanceComputer::GetElongationFactor() const
{
    return elongation_;
}

} // namespace detection
