//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentServer.h"

#include "Entities/MIL_EntityManager.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Entities/Orders/Lima/MIL_LimaManager.h"
#include "Entities/Orders/Limit/MIL_LimitManager.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"

#include "Meteo/PHY_MeteoDataManager.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Hla/HLA_Federate.h"

#include "TER/TER_World.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_ArchiveDirectoryHelper.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT/MT_Archive/MT_InputArchive_ABC.h"

#include "tools/thread/Thread.h"
#include "tools/win32/ProcessMonitor.h"

MIL_AgentServer* MIL_AgentServer::pTheAgentServer_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer constructor
// Created: DFT 02-02-28
// Last modified: JVT 03-09-24
//-----------------------------------------------------------------------------
MIL_AgentServer::MIL_AgentServer( const MIL_Config& config )
    : MT_Timer_ABC             ()
    , nSimState_               ( eSimLoading )
    , config_                  ( config )
    , nTimeStepDuration_       ( 1 )
    , nTimeFactor_             ( 1 )
    , nCurrentTimeStep_        ( 1 )
    , nSimTime_                ( 0 )
    , nCurrentTimeStepRealTime_( 0 )
    , nSimStartTime_           ( MIL_Tools::GetRealTime() )
    , pEntityManager_          ( 0 )
    , pWorkspaceDIA_           ( 0 )
    , pMeteoDataManager_       ( 0 )
    , timerManager_            ()
    , pLimaManager_            ( new MIL_LimaManager() )
    , pLimitManager_           ( new MIL_LimitManager() )
    , pPathFindManager_        ( 0 )
    , pProfilerMgr_            ( new MIL_ProfilerMgr( config.IsProfilingEnabled() ) )
    , pCheckPointManager_      ( 0 )
    , pAgentServer_            ( 0 )
    , pFederate_               ( 0 )
    , pProcessMonitor_         ( new ProcessMonitor() )
{
    assert( !pTheAgentServer_ );
    pTheAgentServer_ = this;
    
    MIL_InputArchive archive;
    archive.AddWarningStream( std::cout );
    archive.Open( config.GetConfigFileName() );

    config_.AddFileToCRC( config.GetConfigFileName() );
    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Initialize
// Created: NLD 2004-09-08
// Modified: JVT 2005-03-07 ( changing initialization order... )
// -----------------------------------------------------------------------------
void MIL_AgentServer::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "Scipio" );
    ReadStaticData ( archive );
    if( config_.UseCheckPoint() )
        pCheckPointManager_->LoadCheckPoint( config_.GetCheckPointFileName() );
    else
        ReadODB( archive );

    if( pFederate_ )
        pEntityManager_->RegisterLocalAgents( *pFederate_ );

    timerManager_.Register( *this );
    timerManager_.Register( *pCheckPointManager_ );

    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    MT_LOG_STARTUP_MESSAGE( "---- SIM Initialized ----" );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
}


//-----------------------------------------------------------------------------
// Name: MIL_AgentServer destructor
// Created: DFT 02-02-28
// Last modified: JVT 03-06-06
//-----------------------------------------------------------------------------
MIL_AgentServer::~MIL_AgentServer()
{
    MT_LOG_INFO_MSG( "Terminating Simulation..." );
    timerManager_.Unregister( *pCheckPointManager_ );
    timerManager_.Unregister( *this );

    MT_LOG_INFO_MSG( "Terminating HLA" );
    delete pFederate_;
    MT_LOG_INFO_MSG( "Terminating pathfind threads" );
    delete pPathFindManager_;
    // $$$$ AGE 2005-02-21:
//    MT_LOG_INFO_MSG( "Cleaning up simulation data" );
//    delete pEntityManager_;
//    delete pLimaManager_;
//    delete pLimitManager_;
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

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadODB
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void MIL_AgentServer::ReadODB( MIL_InputArchive& archive )
{
    assert( !pEntityManager_ );

    archive.Section( "SIM" );

    pEntityManager_     = new MIL_EntityManager    ( archive );
    pCheckPointManager_ = new MIL_CheckPointManager( archive );
    
    archive.ReadField( "TimeFactor", nTimeFactor_, CheckValueGreater( 0 ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Simulation acceleration factor : %d", nTimeFactor_ ) );    
        
    uint nAutoStart = 0;
    if( archive.ReadField( "AutoStart", nAutoStart, MIL_InputArchive::eNothing ) && nAutoStart == 1 )
        ResumeSim();
    archive.EndSection(); // SIM

    archive.Section( "Donnees" );
    pEntityManager_->ReadODB( archive );
    archive.EndSection(); // Donnees
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadStaticData
// Created: JVT 2005-03-07
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadStaticData( MIL_InputArchive& archive )
{   
    nSimState_ = eSimPaused;
    archive.Section( "SIM" );
    archive.ReadField( "TimeStep"  , nTimeStepDuration_ );
    MT_LOG_INFO_MSG( MT_FormatString( "Simulation tick duration : %d seconds", nTimeStepDuration_ ) );
    archive.EndSection(); // SIM
    
    archive.Section( "Donnees" );
    
    pAgentServer_ = new NET_AgentServer( archive );

    MIL_IDManager::Initialize( archive );
    ReadTerData     ( archive );
    ReadMissionsData( archive );

    pMeteoDataManager_ = new PHY_MeteoDataManager( archive );
    pWorkspaceDIA_     = new DEC_Workspace       ( archive );

    MIL_EntityManager::Initialize( archive );

    ReadPathFindData( archive );
    ReadHLA( archive );

    archive.EndSection(); // Donnees
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadPathFindData
// Created: AGE 2005-03-01
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadPathFindData( MIL_InputArchive& archive )
{
    std::string strPathfindFile;
    archive.ReadField( "Pathfind", strPathfindFile );
    MIL_InputArchive pathfindArchive;
    pathfindArchive.AddWarningStream( std::cout );
    pathfindArchive.Open( strPathfindFile );
    config_.AddFileToCRC( strPathfindFile );

    MT_ArchiveDirectoryHelper directoryChanger( strPathfindFile );
    pPathFindManager_  = new DEC_PathFind_Manager( pathfindArchive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadMissionsData
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadMissionsData( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing missions data" );

    std::string strFile;
    archive.ReadField( "Missions", strFile );

    MIL_InputArchive archiveType;
    archiveType.AddWarningStream( std::cout );
    archiveType.Open( strFile );

    config_.AddFileToCRC( strFile );
    MIL_AutomateMissionType::Initialize( archiveType );

    archiveType.Close();
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadTerData
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
void MIL_AgentServer::ReadTerData( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing terrain" );

    std::string strTerFile;
    archive.ReadField( "Terrain", strTerFile );

    MIL_InputArchive terArchive;
    terArchive.AddWarningStream( std::cout );
    terArchive.Open( strTerFile );
    
    config_.AddFileToCRC( strTerFile );

    {
        MT_ArchiveDirectoryHelper directoryChanger( strTerFile );
        MT_LOG_INFO_MSG( MT_FormatString( "Terrain data directory : %s", MT_GetCurrentDir().c_str() ) );
        TER_World::Initialize( terArchive );
        MT_LOG_INFO_MSG( MT_FormatString( "Terrain size (w x h): %.2fkm x %.2fkm", TER_World::GetWorld().GetWidth() / 1000., TER_World::GetWorld().GetHeight()  / 1000. ) );
    }

    std::string strTerDataPath;
    MT_ExtractFilePath( strTerFile, strTerDataPath );
    config_.AddFileToCRC( strTerDataPath + TER_World::GetWorld().GetGraphFileName() );
    config_.AddFileToCRC( strTerDataPath + TER_World::GetWorld().GetNodeFileName() );
    config_.AddFileToCRC( strTerDataPath + TER_World::GetWorld().GetLinkFileName() );

    terArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadHLA
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadHLA( MIL_InputArchive& archive )
{
    std::string strHLASubArchive;
    archive.ReadField( "HLA", strHLASubArchive );
    MT_LOG_INFO_MSG( "Initializing HLA" );
    
    MIL_InputArchive hlaFile;
    hlaFile.AddWarningStream( std::cout );
    hlaFile.Open( strHLASubArchive );
    config_.AddFileToCRC( strHLASubArchive );
    hlaFile.Section( "HLA" );
    bool bUse = false;
    hlaFile.ReadField( "UseHLA", bUse );
    if( ! bUse )
        return;

    std::string strFederationName;
    hlaFile.ReadField( "Federation", strFederationName );
    std::string strFederateName;
    hlaFile.ReadField( "Federate", strFederateName );

    pFederate_ = new HLA_Federate( strFederateName, nTimeStepDuration_ );
    if( ! pFederate_->Join( strFederationName ) )
    {
        MT_LOG_WARNING_MSG( "Could not join federation '" << strFederationName << "'" );
        delete pFederate_;
        pFederate_ = 0;
    }
    else
        MT_LOG_INFO_MSG( "Connected to federation '" << strFederationName << "'" );
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

    if( ! pAgentServer_->IsThreaded() )
        return;

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
    nSimTime_                 = nCurrentTimeStep_ * nTimeStepDuration_;
    nCurrentTimeStepRealTime_ = MIL_Tools::GetRealTime();
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
    pMeteoDataManager_->Update();
    MT_LOG_INFO_MSG( MT_FormatString( "**************** Time tick %d - Profiling (K/D/A/E/S) : %.2fms %.2fms %.2fms %.2fms %.2fms - PathFind : %d short - %d long", nCurrentTimeStep_, pEntityManager_->GetKnowledgesTime(), pEntityManager_->GetDecisionsTime(), pEntityManager_->GetActionsTime(), pEntityManager_->GetEffectsTime(), pEntityManager_->GetStatesTime(), pPathFindManager_->GetNbrShortRequests(), pPathFindManager_->GetNbrLongRequests() ) );
    if( pProcessMonitor_->MonitorProcess() )
        MT_LOG_INFO_MSG( MT_FormatString( "**************** System Status : Memory : %dB (%dB), Virtual Memory : %dB (%dB)\n", pProcessMonitor_->GetMemory(), pProcessMonitor_->GetMaxMemory(), pProcessMonitor_->GetVirtualMemory(), pProcessMonitor_->GetMaxVirtualMemory() ) );

    pProfilerMgr_->NotifyTickEnd( GetCurrentTimeStep() );
    SendMsgEndTick();

    if( pFederate_ )
        pFederate_->Step();

    pEntityManager_->Clean();
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
    NET_ASN_MsgCtrlBeginTick msgBeginTick;
    msgBeginTick.GetAsnMsg() = GetCurrentTimeStep();
    msgBeginTick.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendMsgEndTick
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendMsgEndTick() const
{
    NET_ASN_MsgCtrlEndTick msgEndTick;
    msgEndTick.GetAsnMsg().current_tick        = GetCurrentTimeStep();
    msgEndTick.GetAsnMsg().tick_duration       = (ASN1INT)pProfilerMgr_->GetLastTickDuration();
    msgEndTick.GetAsnMsg().nb_pathfinds_courts = pPathFindManager_->GetNbrShortRequests();
    msgEndTick.GetAsnMsg().nb_pathfinds_longs  = pPathFindManager_->GetNbrLongRequests ();
    msgEndTick.Send();
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
         << nCurrentTimeStepRealTime_
         << nSimStartTime_
         << pEntityManager_
//         << pWorkspaceDIA_        // uniquement données statiques
//         << pMeteoDataManager_    // données statiques + météo locales gérées par MOS
//         << timerManager_         // pas de données ( MIL_AgentServer::Initialize )
//         << pLimaManager_         // données gérées par MOS
//         << pLimitManager_        // données gérées par MOS
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
         >> nCurrentTimeStepRealTime_
         >> nSimStartTime_
         >> pEntityManager_
//         >> pWorkspaceDIA_
//         >> pMeteoDataManager_
//         >> timerManager_
//         >> pLimaManager_
//         >> pLimitManager_
//         >> pPathFindManager_
//         >> pProfilerMgr_
         >> pCheckPointManager_
//         >> pAgentServer_
//         >> pFederate_
    ;

    MT_LOG_INFO_MSG( MT_FormatString( "Simulation acceleration factor : %d", nTimeFactor_ ) );

    nSimState_ = eSimPaused;
    if( nSimState == eSimRunning )
        ResumeSim();
}
