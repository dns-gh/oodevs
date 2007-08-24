// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Dispatcher.h"

#include "Model.h"
#include "SimulationNetworker.h"
#include "ClientsNetworker.h"
#include "ProfileManager.h"
#include "SimulationDispatcher.h"
#include "PluginFactory.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config )
    : config_( config )
    , pModel_( new Model( config_ ) )
{
    pClientsNetworker_   .reset( new ClientsNetworker   ( *this, config_ ) );
    pSimulationNetworker_.reset( new SimulationNetworker( *pModel_, *pClientsNetworker_, handler_, config_ ) );
    pProfileManager_     .reset( new ProfileManager     ( *pModel_, *pClientsNetworker_, config_ ) );

    handler_.Add( new SimulationDispatcher( *pClientsNetworker_, *pModel_ ) );
    handler_.Add( pProfileManager_ );
    handler_.Add( pClientsNetworker_ );
    handler_.Add( pModel_ );

    PluginFactory factory( config_, *pModel_, *pSimulationNetworker_ );
    factory.RegisterPlugins( handler_ );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::~Dispatcher()
{
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Dispatcher::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::Update()
{
    assert( pClientsNetworker_    );
    assert( pSimulationNetworker_ );

    pClientsNetworker_   ->Update();
    pSimulationNetworker_->Update();
}

