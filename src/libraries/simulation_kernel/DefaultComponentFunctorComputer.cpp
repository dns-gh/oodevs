// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultComponentFunctorComputer.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultComponentFunctorComputer::DefaultComponentFunctorComputer( OnComponentFunctor_ABC& componentOperator )
    : componentOperator_( componentOperator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::~DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultComponentFunctorComputer::~DefaultComponentFunctorComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::ApplyOnConpenent
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultComponentFunctorComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    componentOperator_( component );
}
