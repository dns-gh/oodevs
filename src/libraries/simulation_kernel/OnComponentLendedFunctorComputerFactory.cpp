// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory.h"
#include "simulation_kernel/DefaultComponentLendedFunctorComputer.h"

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory constructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
OnComponentLendedFunctorComputerFactory::OnComponentLendedFunctorComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory destructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
OnComponentLendedFunctorComputerFactory::~OnComponentLendedFunctorComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory Create
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
std::auto_ptr< OnComponentLendedFunctorComputer_ABC > OnComponentLendedFunctorComputerFactory::Create( OnComponentFunctor_ABC& componentOperator ) const
{
    return std::auto_ptr< OnComponentLendedFunctorComputer_ABC >( new DefaultComponentLendedFunctorComputer( componentOperator ) );
}

//@TODO $$$$ MGD Replace create by an Apply( Container& container ) to correctly execute