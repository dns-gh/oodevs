// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: MIL_IDManager constructor
// Created: SBO 2009-12-14
// -----------------------------------------------------------------------------
MIL_IDManager::MIL_IDManager()
    : last_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::Destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_IDManager::~MIL_IDManager(){}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::GetFreeId
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
unsigned long MIL_IDManager::GetFreeId()
{
    return last_++;
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::Lock
// Created: SBO 2009-12-14
// -----------------------------------------------------------------------------
void MIL_IDManager::Lock( unsigned long id )
{
    if( id >= last_ )
        last_ = id + 1;
}
