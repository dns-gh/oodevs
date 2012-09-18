// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "tools/IdManager.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: IdManager constructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
IdManager::IdManager()
    : max_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdManager destructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
IdManager::~IdManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdManager::GetNextId
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
unsigned long IdManager::GetNextId()
{
    return max_++;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Reset
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void IdManager::Reset()
{
    max_ = 1;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Lock
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void IdManager::Lock( unsigned long id )
{
    if( id >= max_ )
        max_ = id + 1;
}
