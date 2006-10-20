// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IdManager.h"

// -----------------------------------------------------------------------------
// Name: IdManager constructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
IdManager::IdManager()
    : currentId_( 1 )
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
    return currentId_++;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Reset
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void IdManager::Reset()
{
    currentId_ = 1;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Lock
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void IdManager::Lock( unsigned long id )
{
    if( id >= currentId_ )
        currentId_ = id + 1;
}
