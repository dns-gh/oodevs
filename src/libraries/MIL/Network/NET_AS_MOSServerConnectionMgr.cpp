//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerConnectionMgr.cpp $
// $Author: Age $
// $Modtime: 13/12/04 15:05 $
// $Revision: 3 $
// $Workfile: NET_AS_MOSServerConnectionMgr.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "NET_AS_MOSServerConnectionMgr.h"

#include "NET_Def.h"
#include "NET_AS_MOSServerMsgMgr.h"
#include "NET_AgentServer.h"
#include "MIL_AgentServer.h"

using namespace DIN;
using namespace NEK;

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerConnectionMgr::NET_AS_MOSServerConnectionMgr( NET_AgentServer& agentServer )
    : NET_AS_MOSServerMgr_ABC( agentServer )
    , pServer_               ( 0 )
{
    DIN_ConnectorHost connector( eConnector_SIM_MOS );

    pConnService_ = new DIN_ConnectionServiceServerUserCbk< NET_AS_MOSServerConnectionMgr >( 
                                      *this
                                    , agentServer_.GetDINEngine()
                                    , connector
                                    , agentServer_.GetConnectionProtocols()
                                    , eConnMagicMOSServerAgentServer
                                    , "Agent server for MOS server"
                                    , false );

    pConnService_->SetCbkOnConnectionReceived( & NET_AS_MOSServerConnectionMgr::OnConnectionReceived    );
    pConnService_->SetCbkOnConnectionFailed  ( & NET_AS_MOSServerConnectionMgr::OnBadConnectionReceived );
    pConnService_->SetCbkOnConnectionLost    ( & NET_AS_MOSServerConnectionMgr::OnConnectionLost        );
    pConnService_->SetCbkOnWantFreeStationID ( & NET_AS_MOSServerConnectionMgr::OnWantFreeStationID     );
    pConnService_->SetCbkOnReleaseStationID  ( & NET_AS_MOSServerConnectionMgr::OnReleaseStationID      );
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerConnectionMgr::~NET_AS_MOSServerConnectionMgr()
{
    if( pServer_ )
        StopServer();

    delete pConnService_;
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::StartServer
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
bool NET_AS_MOSServerConnectionMgr::StartServer()
{
    if( pServer_ )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Server '%s' already started", pServer_->GetName().c_str() ).c_str() );
        assert( false );
        return false;
    }

    NEK_AddressINET addr( agentServer_.GetPortAS_MOS() );
    pServer_ = &pConnService_->CreateHost( addr, "Agent server for MOS server" );

    return true;
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::StopServer
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
bool NET_AS_MOSServerConnectionMgr::StopServer()
{
    if( !pServer_ )
    {
        MT_LOG_ERROR_MSG( "Server not started" );
        return false;
    }
    bool bOut = pConnService_->DestroyHost( *pServer_ );
    assert( bOut );

    pServer_ = 0;

    return bOut;
}

//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::OnConnectionReceived
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AS_MOSServerConnectionMgr::OnConnectionReceived( DIN_Server& /*server*/, DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "AS <- MOS - Connection received from %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );
    agentServer_.GetMessageMgr().Enable( link );
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::OnBadConnectionReceived
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AS_MOSServerConnectionMgr::OnBadConnectionReceived( DIN_Server& /*server*/, const NEK_Address_ABC& address, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "AS <- MOS - Bad connection received from %s (reason : %s)", address.GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::OnConnectionLost
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AS_MOSServerConnectionMgr::OnConnectionLost( DIN_Server& /*server*/, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "AS <- MOS - Connection to %s lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
    agentServer_.GetMessageMgr().Disable( link );
    RemoveConnection( link );
    agentServer_.GetMessageMgr().DeleteMessagesFrom( link );
    // $$$$ AGE 2005-06-23: MsgManager.TrashMessagesFrom( link );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::OnWantFreeStationID
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
DIN_StationID NET_AS_MOSServerConnectionMgr::OnWantFreeStationID( DIN_StationID nForcedStationID )
{
    if( agentServer_.GetDINEngine().GetStationID() == DIN_NULL_STATION_ID )
    {
        assert( nForcedStationID == DIN_NULL_STATION_ID );
        // NB : affectation de l'id locale
        DIN_StationID nLocalStationID = MIL_AgentServer::GetWorkspace().GetExerciceID() + 5000; //$$$ NLD - Pourquoi j'ai fait ça ??
        bool bOut = stationIDManager_.LockIdentifier( nLocalStationID );
        assert( bOut );
        return nLocalStationID;
    }

    if( nForcedStationID != DIN_NULL_STATION_ID )
    {
        if( stationIDManager_.LockIdentifier( nForcedStationID ) )
            return nForcedStationID;
        else
            return DIN_NULL_STATION_ID;
    }        
    else
        return stationIDManager_.GetFreeIdentifier();
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::OnReleaseStationID
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void NET_AS_MOSServerConnectionMgr::OnReleaseStationID( DIN_StationID nStationID )
{
    stationIDManager_.ReleaseIdentifier( nStationID );
}

//=============================================================================
// TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::AddConnection
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
NET_AS_MOSServer& NET_AS_MOSServerConnectionMgr::AddConnection( DIN::DIN_Link& link, NET_AS_MOSServer::E_ClientType nClientType )
{
    NET_AS_MOSServer* pConnection = new NET_AS_MOSServer( link, nClientType );
    bool bOut = false;
    switch( nClientType )
    {
        case NET_AS_MOSServer::eMos     : bOut = mosConnectionMap_     .insert( std::make_pair( link.GetStationID(), pConnection ) ).second; break;
        case NET_AS_MOSServer::eMosLight: bOut = mosLightConnectionMap_.insert( std::make_pair( link.GetStationID(), pConnection ) ).second; break;
        default:
            assert( false );
    }

    assert( bOut );
    return *pConnection;
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::RemoveConnection
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerConnectionMgr::RemoveConnection( DIN::DIN_Link& link )
{
    NET_AS_MOSServer* pConnection = GetMosConnectionFromLink( link );
    if( !pConnection )
        return;

    int nOut = 0;
    switch( pConnection->GetClientType() )
    {
        case NET_AS_MOSServer::eMos     : nOut = mosConnectionMap_     .erase( link.GetStationID() ); break;
        case NET_AS_MOSServer::eMosLight: nOut = mosLightConnectionMap_.erase( link.GetStationID() ); break;
        default:
            assert( false ); 
    }
    assert( nOut == 1 );
    delete pConnection;
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::NeedsUpdating
// Created: AGE 2004-10-20
// -----------------------------------------------------------------------------
bool NET_AS_MOSServerConnectionMgr::NeedsUpdating() const
{
    const unsigned int nMaxPending = 10;
    unsigned int nPending = 0;
    for( CIT_MosConnectionMap it = mosConnectionMap_.begin(); it != mosConnectionMap_.end() && nPending < nMaxPending; ++it )
    {
        assert( it->second );
        const NET_AS_MOSServer& server = *it->second;
        nPending += server.GetLink().GetPendingMessages();
    }
    for( CIT_MosConnectionMap it = mosLightConnectionMap_.begin(); it != mosLightConnectionMap_.end() && nPending < nMaxPending; ++it )
    {
        assert( it->second );
        const NET_AS_MOSServer& server = *it->second;
        nPending += server.GetLink().GetPendingMessages();
    }
    return nPending >= nMaxPending;
}
