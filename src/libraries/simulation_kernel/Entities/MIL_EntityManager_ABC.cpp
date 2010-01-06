// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EntityManager_ABC.h"

namespace
{
    MIL_EntityManager_ABC* singleton_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager_ABC constructor
// Created: LDC 2010-01-06
// -----------------------------------------------------------------------------
MIL_EntityManager_ABC::MIL_EntityManager_ABC()
{
    if( singleton_ )
        throw std::runtime_error( "EntityManager already defined" );
    singleton_ = this;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager_ABC destructor
// Created: LDC 2010-01-06
// -----------------------------------------------------------------------------
MIL_EntityManager_ABC::~MIL_EntityManager_ABC()
{
    singleton_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager_ABC::GetSingleton
// Created: LDC 2010-01-06
// -----------------------------------------------------------------------------
MIL_EntityManager_ABC& MIL_EntityManager_ABC::GetSingleton()
{
    if( !singleton_ )
        throw std::runtime_error( "EntityManager not defined" );
    return *singleton_;
}
