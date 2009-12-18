// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/OnComponentFunctorComputerFactory.h"
#include "simulation_kernel/DefaultComponentFunctorComputer.h"

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory constructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
OnComponentFunctorComputerFactory::OnComponentFunctorComputerFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory destructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
OnComponentFunctorComputerFactory::~OnComponentFunctorComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory Create
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
std::auto_ptr< OnComponentFunctorComputer_ABC > OnComponentFunctorComputerFactory::Create( OnComponentFunctor_ABC& componentOperator ) const
{//@TODO $$$$ MGD NEED This factory?
    std::auto_ptr< OnComponentFunctorComputer_ABC > pDotationComputer( new DefaultComponentFunctorComputer( componentOperator ) );
    return pDotationComputer;
}

//@TODO $$$$ MGD Replace create by an Apply( Container& container ) to correctly execute