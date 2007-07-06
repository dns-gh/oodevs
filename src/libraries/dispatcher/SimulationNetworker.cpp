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

#include "Dispatcher.h"
#include "Simulation.h"
#include "Model.h"
#include "ClientsNetworker.h"
#include "Config.h"
#include "Network_Def.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace tools;
using namespace dispatcher;
using namespace DIN;

static const unsigned int magicCookie_ = 1;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Dispatcher& dispatcher, const Config& config )
    : ClientNetworker_ABC( magicCookie_, config.GetNetworkSimulationParameters() )
    , config_            ( config ) 
    , dispatcher_        ( dispatcher )
    , pSimulation_       ( 0 )
{
    GetMessageService().RegisterReceivedMessage( eMsgSimToClient                           , *this, &SimulationNetworker::OnReceiveMsgSimToClient                            );
    GetMessageService().RegisterReceivedMessage( eMsgSimToMiddle                           , *this, &SimulationNetworker::OnReceiveMsgSimToMiddle                            );
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

    assert( !pSimulation_ );
    pSimulation_ = new Simulation( dispatcher_, GetMessageService(), link, config_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnectionFailed
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnectionFailed( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionFailed( link, reason );

    dispatcher_.GetModel().Reset();
    dispatcher_.GetClientsNetworker().DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnectionLost
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionLost( link, reason );

    assert( pSimulation_ );
    delete pSimulation_;
    pSimulation_ = 0;

    dispatcher_.GetModel().Reset();
    dispatcher_.GetClientsNetworker().DenyConnections();
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnReceiveMsgSimToClient
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::OnReceiveMsgSimToClient( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    assert( pSimulation_ && pSimulation_ == &Simulation::GetSimulationFromLink( linkFrom ) );

    try
    {
        AsnMessageDecoder< ASN1T_MsgsSimToClient, ASN1C_MsgsSimToClient > asnDecoder( input );
        pSimulation_->OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnReceiveMsgSimToMiddle
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::OnReceiveMsgSimToMiddle( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    assert( pSimulation_ && pSimulation_ == &Simulation::GetSimulationFromLink( linkFrom ) );

    try
    {
        AsnMessageDecoder< ASN1T_MsgsSimToMiddle, ASN1C_MsgsSimToMiddle > asnDecoder( input );
        pSimulation_->OnReceive( asnDecoder.GetAsnMsg() );
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
    assert( pSimulation_ );
    try
    {
        pSimulation_->Send( asnMsg );
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
    assert( pSimulation_ );
    try
    {
        pSimulation_->Send( asnMsg );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}
