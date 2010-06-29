// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LoadingComputerFactory.h"
#include "DefaultHumanLoadingTimeComputer.h"
#include "DefaultLoadedStateConsistencyComputer.h"

namespace transport
{

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
LoadingComputerFactory::LoadingComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
LoadingComputerFactory::~LoadingComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory::CreateHumanLoadingTimeComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
std::auto_ptr< HumanLoadingTimeComputer_ABC > LoadingComputerFactory::CreateHumanLoadingTimeComputer()
{
    std::auto_ptr< HumanLoadingTimeComputer_ABC > humanLoadingTimeComputer( new DefaultHumanLoadingTimeComputer() );
    return humanLoadingTimeComputer;
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory::CreateLoadedStateConsistencyComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
std::auto_ptr< LoadedStateConsistencyComputer_ABC > LoadingComputerFactory::CreateLoadedStateConsistencyComputer()
{
    std::auto_ptr< LoadedStateConsistencyComputer_ABC > loadedStateConsistencyComputer( new DefaultLoadedStateConsistencyComputer() );
    return loadedStateConsistencyComputer;
}

}
