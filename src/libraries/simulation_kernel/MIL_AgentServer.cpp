//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentServer.h"

#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "MIL_Folk.h"

#include "Meteo/PHY_MeteoDataManager.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "Tools/MIL_Tools.h"
#include "Hla/HLA_Federate.h"

#include "simulation_terrain/TER_World.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT/MT_Archive/MT_InputArchive_ABC.h"

#include "tools/thread/Thread.h"
#include "tools/win32/ProcessMonitor.h"
#include <xeumeuleu/xml.h>



MIL_AgentServer* MIL_AgentServer::pTheAgentServer_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer constructor
// Created: DFT 02-02-28
// Last modified: JVT 03-09-24
//-----------------------------------------------------------------------------
MIL_AgentServer::MIL_AgentServer( MIL_Config& config )
    : MT_Timer_ABC             ()
    , nSimState_               ( eSimLoading )
    , config_                  ( config )
    , nTimeStepDuration_       ( 1 )
    , nTimeFactor_             ( 1 )
    , nCurrentTimeStep_        ( 1 )
    , nSimTime_                ( 0 )
    , nRealTime_               ( 0 )
    , pEffectManager_          ( new MIL_EffectManager() )
    , pEntityManager_          ( 0 )
    , pWorkspaceDIA_           ( 0 )
    , pMeteoDataManager_       ( 0 )
    , timerManager_            ()
    , pTacticalLineManager_    ( new MIL_TacticalLineManager() )
    , pPathFindManager_        ( 0 )
    , pProfilerMgr_            ( new MIL_ProfilerMgr( config.IsProfilingEnabled() ) )
    , pCheckPointManager_      ( 0 )
    , pAgentServer_            ( 0 )
    , pFederate_               ( 0 )
    , pProcessMonitor_         ( new ProcessMonitor() )
    , pFolk_                   ( new MIL_Folk( config ) )
{
    assert( !pTheAgentServer_ );
    pTheAgentServer_ = this;
    
    config_.AddFileToCRC( config_.GetExerciseFile() );
    
    ReadStaticData();
    
    if( config_.HasCheckpoint() )
        pCheckPointManager_->LoadCheckPoint( config_ );
    else
    {
        // $$$$ NLD 2007-01-11: A nettoyer - pb pEntityManager_ instancié par checkpoint
        pEntityManager_     = new MIL_EntityManager    ( *this, *pEffectManager_, *pProfilerMgr_, pFederate_ );
        pCheckPointManager_ = new MIL_CheckPointManager( config_ );
        pEntityManager_->ReadODB( config_ );                        
        Resume();
    }

    timerManager_.Register( *this );

    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    MT_LOG_STARTUP_MESSAGE( "---- SIM Initialized ----" );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer destructor
// Created: DFT 02-02-28
//-----------------------------------------------------------------------------
MIL_AgentServer::~MIL_AgentServer()
{
    MT_LOG_INFO_MSG( "Terminating Simulation..." );
    timerManager_.Unregister( *this );

    MT_LOG_INFO_MSG( "Terminating HLA" );
    delete pFederate_;
    MT_LOG_INFO_MSG( "Terminating pathfind threads" );
    delete pPathFindManager_;
    // $$$$ AGE 2005-02-21:
//    MT_LOG_INFO_MSG( "Cleaning up simulation data" );
//    delete pEntityManager_;
//    delete pTacticalLineManager_;
//    delete pMeteoDataManager_;
//    MT_LOG_INFO_MSG( "Terminating DirectIA" );
//    delete pWorkspaceDIA_;
//    MT_LOG_INFO_MSG( "Terminating Network" );
//    delete pAgentServer_;
//    delete pProfilerMgr_;
//    delete pCheckPointManager_;
    delete pProcessMonitor_;
    //    MT_LOG_INFO_MSG( "Terminating Terrain" );
//    TER_World::DestroyWorld();
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadStaticData
// Created: JVT 2005-03-07
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadStaticData()
{   
    nSimState_         = eSimPaused;
    nTimeStepDuration_ = config_.GetTimeStep();
    nTimeFactor_       = config_.GetTimeFactor();
    MT_LOG_INFO_MSG( MT_FormatString( "Simulation tick duration : %d seconds", nTimeStepDuration_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Simulation acceleration factor : %d", nTimeFactor_ ) );    
        
    pAgentServer_ = new NET_AgentServer( config_, *this, *this );

    ReadTerData();
    pMeteoDataManager_ = new PHY_MeteoDataManager( config_ );
    pWorkspaceDIA_     = new DEC_Workspace       ( config_ );

    MIL_EntityManager::Initialize( config_, *this, *pEffectManager_ );

    if( !config_.IsDataTestMode() )
        pPathFindManager_ = new DEC_PathFind_Manager( config_ );
    ReadHLA();    
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadTerData
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
void MIL_AgentServer::ReadTerData()
{
    MT_LOG_INFO_MSG( "Initializing terrain" );
    config_.AddFileToCRC( config_.GetTerrainFile() );
    MT_LOG_INFO_MSG( MT_FormatString( "Terrain: %s", config_.GetTerrainFile().c_str() ) );
    TER_World::Initialize( config_.GetTerrainFile() );
    MT_LOG_INFO_MSG( MT_FormatString( "Terrain size (w x h): %.2fkm x %.2fkm", TER_World::GetWorld().GetWidth() / 1000., TER_World::GetWorld().GetHeight()  / 1000. ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadHLA
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadHLA()
{
    if( !config_.IsHLAEnabled() )
        return;

    MT_LOG_INFO_MSG( "Initializing HLA" );

    pFederate_ = new HLA_Federate( config_.GetHLAFederate(), nTimeStepDuration_ );
    if( ! pFederate_->Join( config_.GetHLAFederation() ) )
    {
        MT_LOG_WARNING_MSG( "Could not join federation '" << config_.GetHLAFederation() << "'" );
        delete pFederate_;
        pFederate_ = 0;
    }
    else
        MT_LOG_INFO_MSG( "Connected to federation '" << config_.GetHLAFederation() << "'" );
}

//=============================================================================
// MAIN LOOPS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::Update
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MIL_AgentServer::E_SimState MIL_AgentServer::Update()
{
    timerManager_ .Update();
    WaitForNextStep();
    return nSimState_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::WaitForNextStep
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
void MIL_AgentServer::WaitForNextStep()
{
    assert( pAgentServer_ );
    pAgentServer_    ->Update();
    pPathFindManager_->Update();
    if( pFederate_ )
        pFederate_->Tick();
    DIA_Workspace::Instance().UpdateWorkspace();

    long sleepTime = 100;
    if( nSimState_ == eSimRunning )
    {
        const long nextStep = lastStep_ + ( CLOCKS_PER_SEC * nTimeStepDuration_ ) / nTimeFactor_;
        sleepTime = nextStep - clock();
        if( ( lastStep_ + sleepTime ) > nextStep )
            sleepTime = 100;
    }

    if( sleepTime > 25 )
        tools::thread::Thread::Sleep( sleepTime / 2 ); // $$$$ AGE 2004-12-13:
    else
        tools::thread::Thread::Sleep( 0 );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::OnTimer
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void MIL_AgentServer::OnTimer()
{
    nSimTime_  += nTimeStepDuration_;
    nRealTime_ += nTimeStepDuration_;
    lastStep_ = clock();
    MainSimLoop();
    ++ nCurrentTimeStep_;
}

//-----------------------------------------------------------------------------
// Name:  MIL_AgentServer::mainSimLoop
// Created: JVT 02-06-21
// Last modified: JVT 03-04-03
//-----------------------------------------------------------------------------
void MIL_AgentServer::MainSimLoop()
{
    pProfilerMgr_->NotifyTickBegin( GetCurrentTimeStep() );
    SendMsgBeginTick();
    
    pEntityManager_   ->Update();
    pFolk_            ->Update( nCurrentTimeStep_ * nTimeStepDuration_, nTimeStepDuration_ );
    pMeteoDataManager_->Update( nRealTime_ );

    if( pProcessMonitor_->MonitorProcess() )
    {
        
        MT_LOG_INFO_MSG( MT_FormatString( "**************** Time tick %d - Profiling (K/D/A/E/S) : %.2fms %.2fms (A:%.2f P:%.2f Pop:%.2f) %.2fms %.2fms %.2fms - PathFind : %d short %d long %d done - RAM : %.3f MB / %.3f MB (VM)",
            nCurrentTimeStep_, pEntityManager_->GetKnowledgesTime(), pEntityManager_->GetDecisionsTime(), pEntityManager_->GetAutomatesDecisionTime(), pEntityManager_->GetPionsDecisionTime(), 
            pEntityManager_->GetPopulationsDecisionTime(), pEntityManager_->GetActionsTime(), pEntityManager_->GetEffectsTime(), pEntityManager_->GetStatesTime(), pPathFindManager_->GetNbrShortRequests(), 
            pPathFindManager_->GetNbrLongRequests(), pPathFindManager_->GetNbrTreatedRequests(), pProcessMonitor_->GetMemory() / 1048576., pProcessMonitor_->GetVirtualMemory() / 1048576. ) );
    }
    else
    {
        MT_LOG_INFO_MSG( MT_FormatString( "**************** Time tick %d - Profiling (K/D/A/E/S) : %.2fms %.2fms (A:%.2f P:%.2f Pop:%.2f) %.2fms %.2fms %.2fms - PathFind : %d short %d long %d done",
            nCurrentTimeStep_, pEntityManager_->GetKnowledgesTime(), pEntityManager_->GetDecisionsTime(), pEntityManager_->GetAutomatesDecisionTime(), pEntityManager_->GetPionsDecisionTime(), 
            pEntityManager_->GetPopulationsDecisionTime(), pEntityManager_->GetActionsTime(), pEntityManager_->GetEffectsTime(), pEntityManager_->GetStatesTime(), pPathFindManager_->GetNbrShortRequests(), 
            pPathFindManager_->GetNbrLongRequests(), pPathFindManager_->GetNbrTreatedRequests() ) );
    }

    pProfilerMgr_->NotifyTickEnd( GetCurrentTimeStep() );
    SendMsgEndTick();

    if( pFederate_ )
        pFederate_->Step();

    pEntityManager_->Clean();

    pCheckPointManager_->Update();
}

//=============================================================================
// NETWORK
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendMsgBeginTick
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendMsgBeginTick() const
{
    NET_ASN_MsgControlBeginTick msgBeginTick;
    msgBeginTick().current_tick = GetCurrentTimeStep();
    NET_ASN_Tools::WriteGDH( nRealTime_, msgBeginTick().date_time );
    msgBeginTick.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendMsgEndTick
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendMsgEndTick() const
{
    assert( pProcessMonitor_ );

    NET_ASN_MsgControlEndTick msgEndTick;
    msgEndTick().current_tick      = GetCurrentTimeStep();
    msgEndTick().tick_duration     = (ASN1INT)pProfilerMgr_->GetLastTickDuration();
    msgEndTick().short_pathfinds   = pPathFindManager_->GetNbrShortRequests();
    msgEndTick().long_pathfinds    = pPathFindManager_->GetNbrLongRequests ();
    msgEndTick().memory            = pProcessMonitor_->GetMemory();
    msgEndTick().virtual_memory    = pProcessMonitor_->GetVirtualMemory();
    msgEndTick.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendStateToNewClient
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendStateToNewClient() const
{    
    pEntityManager_->SendStateToNewClient();
    pFolk_->SendStateToNewClient();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::save
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_AgentServer::save( MIL_CheckPointOutArchive& file ) const
{
    file << nSimState_
         << nTimeFactor_
         << nCurrentTimeStep_
         << nSimTime_
         << pEntityManager_
//         << pWorkspaceDIA_        // uniquement données statiques
//         << pMeteoDataManager_    // données statiques + météo locales gérées par MOS
//         << timerManager_         // pas de données ( MIL_AgentServer::Initialize )
//         << pTacticalLineManager_ // plus de limit et de lima
//         << pPathFindManager_     // pas stockés
//         << pProfilerMgr_         // pas de données
         << pCheckPointManager_
//         << pAgentServer_         // moi-même ( static )
//         << pFederate_            // reloadé à la main ( cf. MIL_AgentServer::Initialize )
    ;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::load
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_AgentServer::load( MIL_CheckPointInArchive& file )
{
    E_SimState nSimState;

    file >> nSimState
         >> nTimeFactor_
         >> nCurrentTimeStep_
         >> nSimTime_
         >> pEntityManager_ 
//         >> pWorkspaceDIA_
//         >> pMeteoDataManager_
//         >> timerManager_
//         >> pTacticalLineManager_
//         >> pPathFindManager_
//         >> pProfilerMgr_
         >> pCheckPointManager_
//         >> pAgentServer_
//         >> pFederate_
    ;

    MT_LOG_INFO_MSG( MT_FormatString( "Simulation acceleration factor : %d", nTimeFactor_ ) );

    nSimState_ = eSimPaused;
    if( nSimState == eSimRunning )
        Resume();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::WriteODB( xml::xostream& xos ) const
{
    assert( pEntityManager_ );
    pEntityManager_->WriteODB( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendControlInformation
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendControlInformation() const
{
    NET_ASN_MsgControlInformation message;

    message().current_tick         = GetCurrentTimeStep();
    NET_ASN_Tools::WriteGDH( nInitialRealTime_, message().initial_date_time );
    NET_ASN_Tools::WriteGDH( nRealTime_       , message().date_time );
    message().tick_duration        = GetTimeStepDuration();
    message().time_factor          = nTimeFactor_;
    message().status               = (ASN1T_EnumSimulationState)GetSimState();
    message().checkpoint_frequency = GetCheckPointManager().GetCheckPointFrequency();
    message().send_vision_cones    = GetAgentServer().MustSendUnitVisionCones();
    message().profiling_enabled    = GetProfilerManager().IsProfilingEnabled();
    message.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Stop
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Stop()
{
    NET_ASN_MsgControlStopAck msg;
    if( nSimState_ == eSimStopped )
        msg() = EnumControlErrorCode::error_not_started;
    else
    {
        nSimState_ = eSimStopped;
        MT_Timer_ABC::Stop();
        MT_LOG_INFO_MSG( "Simulation stopped" );
        msg() = EnumControlErrorCode::no_error;
    }
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Pause
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Pause()
{
    NET_ASN_MsgControlPauseAck msg;
    if( nSimState_ != eSimRunning )
        msg() = EnumControlErrorCode::error_already_paused;
    else
    {
        nSimState_ = eSimPaused;
        MT_Timer_ABC::Stop();
        MT_LOG_INFO_MSG( "Simulation paused" );
        msg() = EnumControlErrorCode::no_error;
    }
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Resume
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Resume()
{
    NET_ASN_MsgControlResumeAck msg;
    if( nSimState_ != eSimPaused )
        msg() = EnumControlErrorCode::error_not_paused;
    else
    {
        nSimState_ = eSimRunning;
        MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * nTimeStepDuration_ / nTimeFactor_ ) ) );
        MT_LOG_INFO_MSG( "Simulation resumed" );
        msg() = EnumControlErrorCode::no_error;
    }
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetTimeFactor
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::SetTimeFactor( unsigned timeFactor )
{
    NET_ASN_MsgControlChangeTimeFactorAck msg;
    if( timeFactor == 0 )
        msg().error_code = EnumControlErrorCode::error_invalid_time_factor;
    else
    {
        nTimeFactor_ = timeFactor;
        if( nSimState_ == eSimRunning )
        {
            MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * nTimeStepDuration_ / nTimeFactor_ ) ) );
            MT_LOG_INFO_MSG( MT_FormatString( "Time factor set to %d", nTimeFactor_ ).c_str() )
        }
        msg().error_code = EnumControlErrorCode::no_error;
    }
    msg().time_factor = nTimeFactor_;
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetRealTime
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
void MIL_AgentServer::SetRealTime( const std::string& realTime )
{
    uint secs = 0;
    NET_ASN_Tools::ReadGDH( realTime.c_str(), secs );
    NET_ASN_MsgControlDatetimeChangeAck ack;
    if( secs < nInitialRealTime_ )
        ack() = EnumControlErrorCode::error_invalid_date_time;
    else
    {
        nRealTime_ = secs;
        ack() = EnumControlErrorCode::no_error;
    }
    ack.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::RealTimeToTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
uint MIL_AgentServer::RealTimeToTick( uint rt ) const
{
    return ( rt - nRealTime_ + nSimTime_ ) / nTimeStepDuration_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::TickToRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
uint MIL_AgentServer::TickToRealTime( uint tick ) const
{
    return nTimeStepDuration_ * tick - nSimTime_ + nRealTime_;
}
