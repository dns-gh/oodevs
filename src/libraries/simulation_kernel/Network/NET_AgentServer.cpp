//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AgentServer.cpp $
// $Author: Nld $
// $Modtime: 17/06/05 21:32 $
// $Revision: 11 $
// $Workfile: NET_AgentServer.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_AgentServer.h"
#include "NET_AS_MOSServerMsgMgr.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Config.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

//-----------------------------------------------------------------------------
// Name: NET_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time, NET_Simulation_ABC& simulation )
    : ServerNetworker                ( config.GetNetworkPort() )
    , time_                          ( time )
    , pMsgMgr_                       ( new NET_AS_MOSServerMsgMgr( *this, simulation ) )
    , nUnitVisionConesChangeTimeStep_( 0 )
    , bSendUnitVisionCones_          ( false )
{
    MT_LOG_INFO_MSG( "Starting simulation server on port " << config.GetNetworkPort() );
    AllowConnections();
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::~NET_AgentServer()
{
    delete pMsgMgr_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Update
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Update()
{
    try
    {
        ServerNetworker::Update();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Receiving message : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionSucceeded( const std::string& endpoint )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << endpoint << "'" );
    ServerNetworker::ConnectionSucceeded( endpoint );
    // should allow only one connection so ...
    DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << address << "' (" << error << ")" );
    ServerNetworker::ConnectionFailed( address, error );
    pMsgMgr_->RemoveClient( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionError( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' lost (" << error << ")" );
    ServerNetworker::ConnectionError( address, error );
    pMsgMgr_->RemoveClient( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::SetMustSendUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void NET_AgentServer::SetMustSendUnitVisionCones( bool bEnable )
{
    nUnitVisionConesChangeTimeStep_ = time_.GetCurrentTick();
    bSendUnitVisionCones_           = bEnable;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustInitUnitVisionCones
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustInitUnitVisionCones() const
{
    return bSendUnitVisionCones_ && time_.GetCurrentTick() == nUnitVisionConesChangeTimeStep_ + 1;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustSendUnitVisionCones
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustSendUnitVisionCones() const
{
    return bSendUnitVisionCones_;
}
