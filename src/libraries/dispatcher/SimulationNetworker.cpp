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
#include "AsnMessageDecoder.h"
#include "AsnMessageEncoder.h"

using namespace dispatcher;
using namespace DIN;
using namespace NEK;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Dispatcher& dispatcher, const std::string& strHostName, unsigned short nPort )
    : Networker_ABC     ( dispatcher )
    , connectionService_( *this, dinEngine_, DIN_ConnectorGuest(), DIN_ConnectionProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 ), 2 )
    , messageService_   ( *this, dinEngine_, DIN_ConnectorGuest() )
    , pSimulation_      ( 0 )
    , simulationAddress_( strHostName, nPort )
{
    messageService_.SetCbkOnError( &SimulationNetworker::OnErrorReceivingMessage );
    messageService_.RegisterReceivedMessage( eMsgOutSim                                , *this, &SimulationNetworker::OnReceiveMsgOutSim                                 );
    messageService_.RegisterReceivedMessage( eMsgInit                                  , *this, &SimulationNetworker::OnReceiveMsgInit                                   );
    messageService_.RegisterReceivedMessage( eMsgProfilingValues                       , *this, &SimulationNetworker::OnReceiveMsgProfilingValues                        );
    messageService_.RegisterReceivedMessage( eMsgTrace                                 , *this, &SimulationNetworker::OnReceiveMsgTrace                                  );
    messageService_.RegisterReceivedMessage( eMsgUnitVisionCones                       , *this, &SimulationNetworker::OnReceiveMsgUnitVisionCones                        );
    messageService_.RegisterReceivedMessage( eMsgUnitInterVisibility                   , *this, &SimulationNetworker::OnReceiveMsgUnitInterVisibility                    );
    messageService_.RegisterReceivedMessage( eMsgObjectInterVisibility                 , *this, &SimulationNetworker::OnReceiveMsgObjectInterVisibility                  );
    messageService_.RegisterReceivedMessage( eMsgPopulationConcentrationInterVisibility, *this, &SimulationNetworker::OnReceiveMsgPopulationConcentrationInterVisibility );
    messageService_.RegisterReceivedMessage( eMsgPopulationFlowInterVisibility         , *this, &SimulationNetworker::OnReceiveMsgPopulationFlowInterVisibility          );
    messageService_.RegisterReceivedMessage( eMsgKnowledgeGroup                        , *this, &SimulationNetworker::OnReceiveMsgKnowledgeGroup                         );
    messageService_.RegisterReceivedMessage( eMsgArmy                                  , *this, &SimulationNetworker::OnReceiveMsgArmy                                   );
    messageService_.RegisterReceivedMessage( eMsgDebugDrawPoints                       , *this, &SimulationNetworker::OnReceiveMsgDebugDrawPoints                        );
    messageService_.RegisterReceivedMessage( eMsgEnvironmentType                       , *this, &SimulationNetworker::OnReceiveMsgEnvironmentType                        );
    messageService_.RegisterReceivedMessage( eMsgPopulationCollision                   , *this, &SimulationNetworker::OnReceiveMsgPopulationCollision                    );

    connectionService_.SetCbkOnConnectionSuccessful( &SimulationNetworker::OnConnected      );
    connectionService_.SetCbkOnConnectionFailed    ( &SimulationNetworker::OnNotConnected   );
    connectionService_.SetCbkOnConnectionLost      ( &SimulationNetworker::OnConnectionLost );
    connectionService_.JoinHost( simulationAddress_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::~SimulationNetworker()
{
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
    MT_LOG_INFO_MSG( MT_FormatString( "Connected to simulation '%s'", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    assert( !pSimulation_ );
    pSimulation_ = new Simulation( dispatcher_, messageService_, link );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnNotConnected
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnNotConnected( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    assert( !pSimulation_ );
    MT_LOG_INFO_MSG( MT_FormatString( "Connection to simulation '%s' failed (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
    connectionService_.JoinHost( simulationAddress_ );

    dispatcher_.GetModel().Reset();
    dispatcher_.GetClientsNetworker().DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnConnectionLost
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void SimulationNetworker::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    assert( pSimulation_ );
    delete pSimulation_;
    pSimulation_ = 0;

    MT_LOG_INFO_MSG( MT_FormatString( "Connection to simulation '%s' lost (reason : %s) - reconnecting", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );    
    connectionService_.JoinHost( simulationAddress_ );

    dispatcher_.GetModel().Reset();
    dispatcher_.GetClientsNetworker().DenyConnections();
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

//$$$ TMP MESSAGES DIN
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
DECLARE_DIN_CALLBACK( KnowledgeGroup                         )
DECLARE_DIN_CALLBACK( Army                                   )
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
        AsnMessageDecoder< ASN1T_MsgsOutSim, ASN1C_MsgsOutSim > asnDecoder( input );
        pSimulation_->OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& /*exception*/ )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnErrorReceivingMessage
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool SimulationNetworker::OnErrorReceivingMessage( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Error while receiving message from simulation '%s' : %s", link.GetRemoteAddress().GetAddressAsString().c_str(), info.GetInfo().c_str() ).c_str() );
    return false;    
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
        AsnMessageEncoder< ASN1T_MsgsInSim, ASN1C_MsgsInSim > asnEncoder( messageService_, asnMsg );
        pSimulation_->Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& /*exception*/ )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Dispatch
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Dispatch( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pSimulation_ );

    DIN_BufferedMessage copiedMsg( messageService_ );
    copiedMsg.GetOutput().Append( dinMsg.GetBuffer( 0 ), dinMsg.GetAvailable() );

    pSimulation_->Send( nMsgID, copiedMsg );
}
