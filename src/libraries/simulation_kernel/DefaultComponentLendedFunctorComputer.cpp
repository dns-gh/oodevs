// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultComponentLendedFunctorComputer.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultComponentLendedFunctorComputer::DefaultComponentLendedFunctorComputer( OnComponentFunctor_ABC& componentOperator )
    : componentOperator_( componentOperator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::~DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultComponentLendedFunctorComputer::~DefaultComponentLendedFunctorComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::ApplyOnConpenent
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultComponentLendedFunctorComputer::ApplyOnLendedComponent( PHY_ComposantePion& component )
{
    componentOperator_( component );
}
