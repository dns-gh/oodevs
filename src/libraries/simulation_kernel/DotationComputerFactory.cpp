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

using namespace dotation;

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DotationComputerFactory::DotationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DotationComputerFactory::~DotationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< DotationComputer_ABC > DotationComputerFactory::Create() const
{
    return std::auto_ptr< DotationComputer_ABC >( new DefaultDotationComputer() );
}