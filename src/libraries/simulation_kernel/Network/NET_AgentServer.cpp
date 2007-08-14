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

#include "NET_AS_MOSServerConnectionMgr.h"
#include "NET_AS_MOSServerMsgMgr.h"
#include "Tools/MIL_Config.h"
#include "DIN/DIN_EventManager.h"
#include "tools/win32/Win32Exception.h"
#include "MIL_AgentServer.h"

using namespace NEK;
using namespace DIN;
using namespace tools::thread;

//-----------------------------------------------------------------------------
// Name: NET_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time, NET_Simulation_ABC& simulation )
    : time_                          ( time )
    , dinEngine_                     ()
    , bThreaded_                     ( config.IsThreadedNetwork() )
    , nPortAS_MOS_                   ( config.GetNetworkPort() )
    , bTerminated_                   ( false )
    , pMsgMgr_                       ( new NET_AS_MOSServerMsgMgr( *this, simulation ) )
    , pConnectionMgr_                ( new NET_AS_MOSServerConnectionMgr( dinEngine_, *pMsgMgr_, nPortAS_MOS_ ) )
    , nUnitVisionConesChangeTimeStep_( 0 )
    , bSendUnitVisionCones_          ( false )
{
    Start();
    if( bThreaded_ )
    {
        MT_LOG_INFO_MSG( "Using threaded network" );
        Thread::Start();
    }
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::~NET_AgentServer()
{
    Stop();
    delete pMsgMgr_;
    delete pConnectionMgr_;
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Start
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Start()
{   
    pConnectionMgr_->StartServer();

    // Initialize asn
    ASN1OCTET buffer[128];
    ASN1PERDecodeBuffer decodeBuffer( buffer, 128, TRUE );
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Stop
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Stop()
{
    bTerminated_ = true;
    pConnectionMgr_->StopServer();
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Update
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Update()
{
    if( ! bThreaded_ )
        do
        {
            MT_CriticalSectionLocker locker( dinEngineCriticalSection_ );
            dinEngine_.Update();
        }
        while( pConnectionMgr_->NeedsUpdating() );

    // Perform synchronous updates
    pMsgMgr_->DoUpdate();
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::Run
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
void NET_AgentServer::Run()
{
    assert( bThreaded_ );
    while( ! bTerminated_ )
    {
        do
        {
            {
                MT_CriticalSectionLocker locker( dinEngineCriticalSection_ );
                dinEngine_.Update();
            }
            Yield();
        }
        while( dinEngine_.GetEventManager().GetPendingEvents() > 10 || pConnectionMgr_->NeedsUpdating() );
        Sleep( 25 );
    }
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
// Name: NET_AgentServer::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void NET_AgentServer::OnUnexpected( Win32Exception& e )
{
    MT_LOG_ERROR_MSG( "Win32 Exception caught in network thread : " << e.what() );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void NET_AgentServer::OnUnexpected( std::exception& e )
{
    MT_LOG_ERROR_MSG( "Exception caught in network thread : " << e.what() );
    MT_CriticalSectionLocker locker( dinEngineCriticalSection_ );
    dinEngine_.~DIN_Engine();
    new (&dinEngine_) DIN_Engine();
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void NET_AgentServer::OnUnexpected()
{
    MT_LOG_ERROR_MSG( "Unknown exception caught in network thread" );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::GetMessageMgr
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
NET_Publisher_ABC& NET_AgentServer::GetMessageMgr() const
{
    assert( pMsgMgr_ );
    return *pMsgMgr_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::GetConcreteMessageMgr
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr& NET_AgentServer::GetConcreteMessageMgr() const
{
    assert( pMsgMgr_ );
    return *pMsgMgr_;
}
