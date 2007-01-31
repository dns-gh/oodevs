// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "Master.h"

#include "ClientsNetworker.h"
#include "DataManager.h"
#include "ProfileManager.h"
#include "Config.h"

using namespace master;

// -----------------------------------------------------------------------------
// Name: Master constructor
// Created: NLD 2007-01-25
// -----------------------------------------------------------------------------
Master::Master( Config& config )
    : clientsNetworker_( new ClientsNetworker( *this, config.GetMasterFile() ) )
    , dataManager_     ( new DataManager     ( *this, config ) )
    , profileManager_  ( new ProfileManager  ( *this, config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Master destructor
// Created: NLD 2007-01-25
// -----------------------------------------------------------------------------
Master::~Master()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Master::Update
// Created: NLD 2007-01-25
// -----------------------------------------------------------------------------
void Master::Update()
{
    clientsNetworker_->Update();
}

// -----------------------------------------------------------------------------
// Name: Master::GetDataManager
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const DataManager& Master::GetDataManager() const
{
    return *dataManager_;
}

// -----------------------------------------------------------------------------
// Name: Master::GetProfileManager
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
const ProfileManager& Master::GetProfileManager() const
{
    return *profileManager_;
}
