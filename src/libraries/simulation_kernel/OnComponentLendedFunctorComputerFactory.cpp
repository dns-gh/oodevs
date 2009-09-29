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
: pDotationComputer_( new DefaultComponentLendedFunctorComputer() )
{
    //NOTHING
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
OnComponentLendedFunctorComputer_ABC& OnComponentLendedFunctorComputerFactory::Create( OnComponentFunctor_ABC& componentOperator ) const
{
    pDotationComputer_->Reset( componentOperator );
    return *pDotationComputer_;
}

//@TODO MGD Replace create by an Apply( Container& container ) to correctly execute