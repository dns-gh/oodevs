// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/ComposantesAbleToBeFiredComputerFactory.h"
#include "simulation_kernel/DefaultComposantesAbleToBeFiredComputer.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: ComposantesAbleToBeFiredComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
ComposantesAbleToBeFiredComputerFactory::ComposantesAbleToBeFiredComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComposantesAbleToBeFiredComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
ComposantesAbleToBeFiredComputerFactory::~ComposantesAbleToBeFiredComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComposantesAbleToBeFiredComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< ComposantesAbleToBeFiredComputer_ABC > ComposantesAbleToBeFiredComputerFactory::Create( bool bFireOnlyOnMajorComposantes /*=false*/ ) const
{
    return std::auto_ptr< ComposantesAbleToBeFiredComputer_ABC >( new DefaultComposantesAbleToBeFiredComputer( bFireOnlyOnMajorComposantes ) );
}
