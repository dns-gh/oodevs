// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultDotationComputer.h"
#include "simulation_kernel/ConsumptionComputer_ABC.h"
#include "simulation_kernel/ConsumptionOperator_ABC.h"

namespace dotation
{

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultDotationComputer::DefaultDotationComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::~DefaultDotationComputer
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
DefaultDotationComputer::~DefaultDotationComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::ApplyOnConpenent
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultDotationComputer::Reset( ConsumptionOperator_ABC& consumptionOperator )
{
    pConsumptionOperator_ = &consumptionOperator;
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::ApplyOnConpenent
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultDotationComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    (*pConsumptionOperator_)( component );
}

} // namespace
