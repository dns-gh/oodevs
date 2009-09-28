// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DotationComputerFactory.h"
#include "simulation_kernel/DefaultDotationComputer.h"

namespace dotation
{

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory constructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
DotationComputerFactory::DotationComputerFactory()
: pDotationComputer_( new DefaultDotationComputer() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory destructor
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
DotationComputerFactory::~DotationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory Create
// Created: MGD 2009-08-24
// -----------------------------------------------------------------------------
DotationComputer_ABC& DotationComputerFactory::Create( ConsumptionOperator_ABC& consumptionOperator ) const
{
    pDotationComputer_->Reset( consumptionOperator );
    return *pDotationComputer_;
}

//@TODO MGD Replace create by an Apply( Container& container ) to correctly execute

} //namespace dotation