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
#include "Config.h"
#include "SimulationDispatcher.h"
#include "SaverFacade.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config )
    : config_( config )
    , pModel_( new Model() )
{
    pClientsNetworker_   .reset( new ClientsNetworker   ( *this, config_ ) );
    pSimulationNetworker_.reset( new SimulationNetworker( *pModel_, *pClientsNetworker_, handler_, config_ ) );
    pProfileManager_     .reset( new ProfileManager     ( *pModel_, *pClientsNetworker_, config_ ) );

    handler_.Add( boost::shared_ptr< MessageHandler_ABC >( new SimulationDispatcher( *pClientsNetworker_, *pModel_ ) ) );
    handler_.Add( pProfileManager_ );
    handler_.Add( pClientsNetworker_ );
    handler_.Add( pModel_ );
    if( config.RecorderEnabled() )
        handler_.Add( boost::shared_ptr< MessageHandler_ABC >( new SaverFacade( *pModel_, config_ ) ) );
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

