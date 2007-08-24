// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationNetworker.h"

#include "Simulation.h"
#include "Model.h"
#include "ClientsNetworker.h"
#include "Config.h"
#include "Network_Def.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace tools;
using namespace dispatcher;
using namespace DIN;

static const unsigned int magicCookie_ = 1;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Model& model, ClientsNetworker& clients, MessageHandler_ABC& handler, const Config& config )
    : ClientNetworker_ABC( magicCookie_, config.GetNetworkSimulationParameters() )
    , model_             ( model )
    , clients_           ( clients )
    , handler_           ( handler )
{
    RegisterMessage( *this, &SimulationNetworker::OnReceiveMsgSimToClient );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::~SimulationNetworker()
{
    // NOTHING
}
  
// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnected
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnected( DIN_Link& link )
{
    ClientNetworker_ABC::OnConnected( link );
    assert( !simulation_.get() );
    simulation_.reset( new Simulation( handler_, GetMessageService(), link ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnectionFailed
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnectionFailed( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionFailed( link, reason );
    model_.Reset();
    clients_.DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnectionLost
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionLost( link, reason );
    simulation_.reset();
    model_.Reset();
    clients_.DenyConnections();
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnReceiveMsgSimToClient
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::OnReceiveMsgSimToClient( DIN::DIN_Link& /*linkFrom*/, const ASN1T_MsgsSimToClient& message )
{
    try
    {
        simulation_->OnReceive( message );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// =============================================================================
// SENT MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Send
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Send( const ASN1T_MsgsClientToSim& asnMsg )
{
    try
    {
        simulation_->Send( asnMsg );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void SimulationNetworker::Send( const ASN1T_MsgsMiddleToSim& asnMsg )
{
    try
    {
        simulation_->Send( asnMsg );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}
