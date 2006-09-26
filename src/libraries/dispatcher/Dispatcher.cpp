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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher()
    : pModel_              ( 0 )
    , pSimulationNetworker_( 0 )
    , pClientsNetworker_   ( 0 )
    , pProfileManager_     ( 0 )
{
    //$$$ TMP
    pModel_               = new Model              ( *this );
    pSimulationNetworker_ = new SimulationNetworker( *this , "localhost", 10000 );
    pClientsNetworker_    = new ClientsNetworker   ( *this , 10001 );
    pProfileManager_      = new ProfileManager     ( *this );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::~Dispatcher()
{
    delete pModel_;
    delete pSimulationNetworker_;
    delete pClientsNetworker_;
    delete pProfileManager_;
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
    assert( pProfileManager_      );
    assert( pClientsNetworker_    );
    assert( pSimulationNetworker_ );

    pProfileManager_     ->Update();
    pClientsNetworker_   ->Update();
    pSimulationNetworker_->Update();
}


// =============================================================================
// DISPATCHING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToSimulation
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToSimulation( const ASN1T_MsgsInSim& asnMsg )
{
    assert( pSimulationNetworker_ );
    pSimulationNetworker_->Dispatch( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToSimulation
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToSimulation( unsigned int nMsgID, DIN::DIN_BufferedMessage& dinMsg )
{
    assert( pSimulationNetworker_ );
    pSimulationNetworker_->Dispatch( nMsgID, dinMsg );   
}

// ----

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToClients
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToClients( const ASN1T_MsgsInClient& asnMsg )
{
    assert( pClientsNetworker_ );
    pClientsNetworker_->Dispatch( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToClients
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToClients( unsigned int nMsgID, DIN::DIN_BufferedMessage& dinMsg )
{
    assert( pClientsNetworker_ );
    pClientsNetworker_->Dispatch( nMsgID, dinMsg );   
}

