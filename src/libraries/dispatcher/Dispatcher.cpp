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
    pClientsNetworker_   .reset( new ClientsNetworker   ( config_, handler_ ) );
    pSimulationNetworker_.reset( new SimulationNetworker( *pModel_, *pClientsNetworker_, handler_, config_ ) );

    handler_.AddHandler( pClientsNetworker_ );
    handler_.AddHandler( pModel_ );

    PluginFactory factory( config_, *pModel_, *pSimulationNetworker_, *pClientsNetworker_ );
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

