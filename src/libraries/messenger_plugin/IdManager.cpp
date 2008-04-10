// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "IdManager.h"

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: IdManager constructor
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
IdManager::IdManager():
    nextId_(0)
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: IdManager::~IdManager
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
IdManager::~IdManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdManager::nexId
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
unsigned int IdManager::nextId()
{
    return nextId_++;
}
