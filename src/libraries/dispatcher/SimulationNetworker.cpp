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
#include "Network_Def.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace tools;
using namespace dispatcher;
using namespace DIN;

namespace 
{
    std::string ReadHost( const std::string& configFile )
    {
        std::string host;
        xml::xifstream xis( configFile );
        xis >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "network" )
                        >> xml::attribute( "client", host );
        return host;
    }
}

static const unsigned int magicCookie_ = 1;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Dispatcher& dispatcher, const std::string& configFile )
    : ClientNetworker_ABC( magicCookie_, ReadHost( configFile ) )
    , dispatcher_        ( dispatcher )
    , pSimulation_       ( 0 )
    , configFile_        ( configFile )
{
    GetMessageService().RegisterReceivedMessage( eMsgSimToClient                           , *this, &SimulationNetworker::OnReceiveMsgSimToClient                            );
    GetMessageService().RegisterReceivedMessage( eMsgSimToMiddle                           , *this, &SimulationNetworker::OnReceiveMsgSimToMiddle                            );
    GetMessageService().RegisterReceivedMessage( eMsgProfilingValues                       , *this, &SimulationNetworker::OnReceiveMsgProfilingValues                        );
    GetMessageService().RegisterReceivedMessage( eMsgTrace                                 , *this, &SimulationNetworker::OnReceiveMsgTrace                                  );
    GetMessageService().RegisterReceivedMessage( eMsgUnitVisionCones                       , *this, &SimulationNetworker::OnReceiveMsgUnitVisionCones                        );
    GetMessageService().RegisterReceivedMessage( eMsgUnitInterVisibility                   , *this, &SimulationNetworker::OnReceiveMsgUnitInterVisibility                    );
    GetMessageService().RegisterReceivedMessage( eMsgObjectInterVisibility                 , *this, &SimulationNetworker::OnReceiveMsgObjectInterVisibility                  );
    GetMessageService().RegisterReceivedMessage( eMsgPopulationConcentrationInterVisibility, *this, &SimulationNetworker::OnReceiveMsgPopulationConcentrationInterVisibility );
    GetMessageService().RegisterReceivedMessage( eMsgPopulationFlowInterVisibility         , *this, &SimulationNetworker::OnReceiveMsgPopulationFlowInterVisibility          );
    GetMessageService().RegisterReceivedMessage( eMsgDebugDrawPoints                       , *this, &SimulationNetworker::OnReceiveMsgDebugDrawPoints                        );
    GetMessageService().RegisterReceivedMessage( eMsgEnvironmentType                       , *this, &SimulationNetworker::OnReceiveMsgEnvironmentType                        );
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
    pSimulation_ = new Simulation( dispatcher_, GetMessageService(), link, configFile_ );
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

#define DECLARE_DIN_CALLBACK( MSG )                                                                \
    void SimulationNetworker::OnReceiveMsg##MSG( DIN::DIN_Link& linkFrom, DIN::DIN_Input& msg )    \
    {                                                                                              \
        assert( pSimulation_ && pSimulation_ == &Simulation::GetSimulationFromLink( linkFrom ) );  \
        pSimulation_->OnReceive( eMsg##MSG, msg );                                                 \
    }

DECLARE_DIN_CALLBACK( ProfilingValues                        )
DECLARE_DIN_CALLBACK( Trace                                  )
DECLARE_DIN_CALLBACK( UnitVisionCones                        )
DECLARE_DIN_CALLBACK( UnitInterVisibility                    )
DECLARE_DIN_CALLBACK( ObjectInterVisibility                  )
DECLARE_DIN_CALLBACK( PopulationConcentrationInterVisibility )
DECLARE_DIN_CALLBACK( PopulationFlowInterVisibility          )
DECLARE_DIN_CALLBACK( DebugDrawPoints                        )
DECLARE_DIN_CALLBACK( EnvironmentType                        )


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
        AsnMessageEncoder< ASN1T_MsgsClientToSim, ASN1C_MsgsClientToSim > asnEncoder( GetMessageService(), asnMsg );
        pSimulation_->Send( asnMsg, asnEncoder.GetDinMsg() );
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
        AsnMessageEncoder< ASN1T_MsgsMiddleToSim, ASN1C_MsgsMiddleToSim > asnEncoder( GetMessageService(), asnMsg );
        pSimulation_->Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Send
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Send( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pSimulation_ );

    DIN_BufferedMessage copiedMsg( GetMessageService() );
    copiedMsg.GetOutput().Append( dinMsg.GetBuffer( 0 ), dinMsg.GetAvailable() );

    pSimulation_->Send( nMsgID, copiedMsg );
}
