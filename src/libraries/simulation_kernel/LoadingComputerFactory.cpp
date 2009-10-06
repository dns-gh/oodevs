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
LoadingComputerFactory::LoadingComputerFactory() :
    humanLoadingTimeComputer_(new DefaultHumanLoadingTimeComputer()),
    loadedStateConsistencyComputer_(new DefaultLoadedStateConsistencyComputer())
{
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
LoadingComputerFactory::~LoadingComputerFactory()
{
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory::CreateHumanLoadingTimeComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
HumanLoadingTimeComputer_ABC& LoadingComputerFactory::CreateHumanLoadingTimeComputer()
{
    humanLoadingTimeComputer_->Reset();
    return *humanLoadingTimeComputer_;
}

// -----------------------------------------------------------------------------
// Name: LoadingComputerFactory::CreateLoadedStateConsistencyComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
LoadedStateConsistencyComputer_ABC& LoadingComputerFactory::CreateLoadedStateConsistencyComputer()
{
    loadedStateConsistencyComputer_->Reset();
    return *loadedStateConsistencyComputer_;
}

}
