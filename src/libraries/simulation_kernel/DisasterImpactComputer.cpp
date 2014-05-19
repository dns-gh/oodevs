// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DisasterImpactComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

// -----------------------------------------------------------------------------
// Name: DisasterImpactComputer constructor
// Created: JSR 2014-05-15
// -----------------------------------------------------------------------------
DisasterImpactComputer::DisasterImpactComputer( double value )
    : modifier_( 1 )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactComputer destructor
// Created: JSR 2014-05-15
// -----------------------------------------------------------------------------
DisasterImpactComputer::~DisasterImpactComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactComputer::ApplyOnComponent
// Created: JSR 2014-05-15
// -----------------------------------------------------------------------------
void DisasterImpactComputer::ApplyOnComponent( PHY_ComposantePion& composante )
{
    if( composante.IsUsable() )
        modifier_ = std::min( modifier_, composante.GetType().GetDisasterImpact( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactComputer::GetModifier
// Created: JSR 2014-05-15
// -----------------------------------------------------------------------------
double DisasterImpactComputer::GetModifier() const
{
    return modifier_;
}
