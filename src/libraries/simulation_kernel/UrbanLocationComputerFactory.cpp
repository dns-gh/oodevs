// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/UrbanLocationComputerFactory.h"
#include "simulation_kernel/DefaultUrbanLocationComputer.h"
#include <urban/TerrainObject_ABC.h>

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: UrbanLocationComputerFactory constructor
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
UrbanLocationComputerFactory::UrbanLocationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLocationComputerFactory destructor
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
UrbanLocationComputerFactory::~UrbanLocationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLocationComputerFactory Create
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
std::auto_ptr< UrbanLocationComputer_ABC > UrbanLocationComputerFactory::Create( const urban::TerrainObject_ABC& block, float deployment ) const
{
    std::auto_ptr< UrbanLocationComputer_ABC > pUrbanLocationComputer( new DefaultUrbanLocationComputer( block, deployment ) );
    return pUrbanLocationComputer;
}