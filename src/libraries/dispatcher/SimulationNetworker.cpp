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
#include "network/AsnMessageDecoder.h"
#include "network/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace network;
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

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Dispatcher& dispatcher, const std::string& configFile )
    : ClientNetworker_ABC( ReadHost( configFile ) )
    , dispatcher_        ( dispatcher )
    , pSimulation_       ( 0 )
{
    GetMessageService().RegisterReceivedMessage( eMsgOutSim                                , *this, &SimulationNetworker::OnReceiveMsgOutSim                                 );
    GetMessageService().RegisterReceivedMessage( eMsgInit                                  , *this, &SimulationNetworker::OnReceiveMsgInit                                   );
    GetMessageService().RegisterReceivedMessage( eMsgProfilingValues                       , *this, &SimulationNetworker::OnReceiveMsgProfilingValues                        );
    GetMessageService().RegisterReceivedMessage( eMsgTrace                                 , *this, &SimulationNetworker::OnReceiveMsgTrace                                  );
    GetMessageService().RegisterReceivedMessage( eMsgUnitVisionCones                       , *this, &SimulationNetworker::OnReceiveMsgUnitVisionCones                        );
    GetMessageService().RegisterReceivedMessage( eMsgUnitInterVisibility                   , *this, &SimulationNetworker::OnReceiveMsgUnitInterVisibility                    );
    GetMessageService().RegisterReceivedMessage( eMsgObjectInterVisibility                 , *this, &SimulationNetworker::OnReceiveMsgObjectInterVisibility                  );
    GetMessageService().RegisterReceivedMessage( eMsgPopulationConcentrationInterVisibility, *this, &SimulationNetworker::OnReceiveMsgPopulationConcentrationInterVisibility );
    GetMessageService().RegisterReceivedMessage( eMsgPopulationFlowInterVisibility         , *this, &SimulationNetworker::OnReceiveMsgPopulationFlowInterVisibility          );
    GetMessageService().RegisterReceivedMessage( eMsgDebugDrawPoints                       , *this, &SimulationNetworker::OnReceiveMsgDebugDrawPoints                        );
    GetMessageService().RegisterReceivedMessage( eMsgEnvironmentType                       , *this, &SimulationNetworker::OnReceiveMsgEnvironmentType                        );
    GetMessageService().RegisterReceivedMessage( eMsgPopulationCollision                   , *this, &SimulationNetworker::OnReceiveMsgPopulationCollision                    );
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
    pSimulation_ = new Simulation( dispatcher_, GetMessageService(), link );
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

DECLARE_DIN_CALLBACK( Init                                   )
DECLARE_DIN_CALLBACK( ProfilingValues                        )
DECLARE_DIN_CALLBACK( Trace                                  )
DECLARE_DIN_CALLBACK( UnitVisionCones                        )
DECLARE_DIN_CALLBACK( UnitInterVisibility                    )
DECLARE_DIN_CALLBACK( ObjectInterVisibility                  )
DECLARE_DIN_CALLBACK( PopulationConcentrationInterVisibility )
DECLARE_DIN_CALLBACK( PopulationFlowInterVisibility          )
DECLARE_DIN_CALLBACK( DebugDrawPoints                        )
DECLARE_DIN_CALLBACK( EnvironmentType                        )
DECLARE_DIN_CALLBACK( PopulationCollision                    )

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnReceiveMsgOutSim
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::OnReceiveMsgOutSim( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    assert( pSimulation_ && pSimulation_ == &Simulation::GetSimulationFromLink( linkFrom ) );

    try
    {
        network::AsnMessageDecoder< ASN1T_MsgsOutSim, ASN1C_MsgsOutSim > asnDecoder( input );
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
// Name: SimulationNetworker::Dispatch
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Dispatch( const ASN1T_MsgsInSim& asnMsg )
{
    assert( pSimulation_ );
    try
    {
        network::AsnMessageEncoder< ASN1T_MsgsInSim, ASN1C_MsgsInSim > asnEncoder( GetMessageService(), asnMsg );
        pSimulation_->Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Dispatch
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Dispatch( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pSimulation_ );

    DIN_BufferedMessage copiedMsg( GetMessageService() );
    copiedMsg.GetOutput().Append( dinMsg.GetBuffer( 0 ), dinMsg.GetAvailable() );

    pSimulation_->Send( nMsgID, copiedMsg );
}
