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

#include "MIL_Pch.h"

#include "NET_AgentServer.h"

#include "NET_Def.h"

#include "NET_AS_MOSServerConnectionMgr.h"
#include "NET_AS_MOSServerMsgMgr.h"
#include "NET_LAU_Mgr.h"
#include "MIL_AgentServer.h"
#include "DIN/DIN_EventManager.h"

using namespace NEK;
using namespace DIN;
using namespace tools::thread;

//-----------------------------------------------------------------------------
// Name: NET_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::NET_AgentServer( MIL_InputArchive& archive )
    : dinEngine_                     (  )
    , nPortAS_MOS_                   ( MIL_DEFAULT_PORT_AS_MOS )
    , nMagicAS_MOS_                  ( eConnMagicMOSServerAgentServer )
    , connectionProtocols_           ( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 )
    , bSendUnitVisionCones_          ( false )
    , bThreaded_                     ( false )
    , bTerminated_                   ( false )
    , nUnitVisionConesChangeTimeStep_( 0 )
{
    std::string strFile;
    archive.ReadField( "Reseau", strFile );

    MIL_InputArchive networkArchive;
    networkArchive.AddWarningStream( std::cout );
    networkArchive.Open( strFile );
    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( strFile );
    
    networkArchive.Section( "Reseau" );

    pConnectionMgr_ = new NET_AS_MOSServerConnectionMgr( *this );
    pMsgMgr_        = new NET_AS_MOSServerMsgMgr       ( *this );     
    pLauMgr_        = new NET_LAU_Mgr                  ( *this, networkArchive );
    
    uint nBasePort;
    networkArchive.Section( "AgentServer-MOSServer" );
    networkArchive.ReadField( "BasePort", nBasePort );
    networkArchive.ReadField( "Magic", nMagicAS_MOS_ );

    nPortAS_MOS_ = (uint16)( nBasePort + MIL_AgentServer::GetWorkspace().GetExerciceID() );    

    networkArchive.EndSection(); // AgentServer-MOSServer

    networkArchive.ReadField( "ThreadReseauActif", bThreaded_ );

    networkArchive.EndSection(); // Reseau
    networkArchive.Close();
    
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
    delete pLauMgr_;
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
    pLauMgr_->Connect( dinEngine_ );

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
    pLauMgr_->Disconnect();
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
    pLauMgr_->DoUpdate();
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
    nUnitVisionConesChangeTimeStep_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    bSendUnitVisionCones_           = bEnable;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustInitUnitVisionCones
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustInitUnitVisionCones() const
{
    return bSendUnitVisionCones_ && MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() == nUnitVisionConesChangeTimeStep_ + 1;
}



