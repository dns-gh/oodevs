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
#include "Tools/MIL_Config.h"
#include "MIL_AgentServer.h"

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
    ASN1OCTET buffer[128];
    ASN1PERDecodeBuffer decodeBuffer( buffer, 128, TRUE );
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
    ServerNetworker::Update();
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionSucceeded( const std::string& endpoint )
{
    ServerNetworker::ConnectionSucceeded( endpoint );
    pMsgMgr_->AddClient( endpoint );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionFailed( const std::string& address, const std::string& error )
{
    ServerNetworker::ConnectionFailed( address, error );
    pMsgMgr_->RemoveClient( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionError( const std::string& address, const std::string& error )
{
    ServerNetworker::ConnectionFailed( address, error );
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
// Name: NET_AgentServer::GetConcreteMessageMgr
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr& NET_AgentServer::GetConcreteMessageMgr() const
{
    return *pMsgMgr_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::GetMessageMgr
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
NET_Publisher_ABC& NET_AgentServer::GetMessageMgr() const
{
    return *pMsgMgr_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustSendUnitVisionCones
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustSendUnitVisionCones() const
{
    return bSendUnitVisionCones_;
}
