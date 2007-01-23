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
#include "xeumeuleu/xml.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( Config& config )
    : pModel_              ( 0 )
    , pSimulationNetworker_( 0 )
    , pClientsNetworker_   ( 0 )
    , pProfileManager_     ( 0 )
{
    std::string profiles;

    xml::xifstream xisMain( config.GetExerciseFile() );
    xisMain >> xml::start( "exercise" )
                >> xml::start( "profiles" )
                    >> xml::attribute( "file", profiles );

    pModel_               = new Model              ( *this );
    pSimulationNetworker_ = new SimulationNetworker( *this, config.GetGameFile() );
    pClientsNetworker_    = new ClientsNetworker   ( *this, config.GetGameFile() );
    pProfileManager_      = new ProfileManager     ( *this, config.BuildExerciseChildFile( profiles ) );
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
    assert( pClientsNetworker_    );
    assert( pSimulationNetworker_ );

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
void Dispatcher::DispatchToSimulation( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pSimulationNetworker_ );
    pSimulationNetworker_->Dispatch( nMsgID, dinMsg );   
}

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
void Dispatcher::DispatchToClients( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pClientsNetworker_ );
    pClientsNetworker_->Dispatch( nMsgID, dinMsg );   
}

