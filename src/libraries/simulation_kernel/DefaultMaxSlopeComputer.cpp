// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultMaxSlopeComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

// -----------------------------------------------------------------------------
// Name: DefaultMaxSlopeComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultMaxSlopeComputer::DefaultMaxSlopeComputer()
    : maxSlope_          ( std::numeric_limits< double >::max() )
    , hasUsableComponent_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultMaxSlopeComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultMaxSlopeComputer::~DefaultMaxSlopeComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultMaxSlopeComputer::ApplyOnComponent
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultMaxSlopeComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    if( component.CanMove() )
    {
        hasUsableComponent_ = true;
        maxSlope_ = std::min( maxSlope_, component.GetType().GetMaxSlope() );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultMaxSlopeComputer::GetMaxSlope
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
double DefaultMaxSlopeComputer::GetMaxSlope() const
{
    return hasUsableComponent_ ? maxSlope_ : 0;
}

}
