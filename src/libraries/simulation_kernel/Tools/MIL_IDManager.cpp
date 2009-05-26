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

unsigned long MIL_IDManager::last_ = 1; 

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::GetFreeId
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
unsigned long MIL_IDManager::GetFreeId()
{
    return last_++;
}
