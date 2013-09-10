//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentServer.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/MIL_EntityManager.h"
#include "Decision/Brain.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Objects/MIL_BurningCells.h"
#include "Entities/Objects/ResourceTools.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "resource_network/ResourceNetworkModel.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Config.h"
#include "tools/ExerciseSettings.h"
#include "tools/Loader_ABC.h"
#include "tools/FileWrapper.h"
#include "Urban/MIL_UrbanCache.h"
#include "Tools/MIL_IDManager.h"
#include <tools/thread/Thread.h>
#include <tools/win32/ProcessMonitor.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/algorithm/string/regex.hpp>

namespace
{
    struct AgentServerInit : boost::noncopyable
    {
        explicit AgentServerInit( MIL_AgentServer*& agent )
            : agent_( agent )
            , done_ ( false )
        {}
        ~AgentServerInit()
        {
            if( !done_ )
                agent_ = 0;
        }
        void Done()
        {
            done_ = true;
        }

    private:
        MIL_AgentServer*& agent_;
        bool done_;
    };

    unsigned long FindMaxIdInFile( const tools::Path& filePath )
    {
        unsigned long maxId = 0;
        try
        {
            if( filePath.Exists() )
            {
                std::string currentLine;
                tools::Ifstream ifile( filePath );
                while( std::getline( ifile, currentLine ) )
                {
                    boost::regex idRegex( "id=\"([0-9]+)\"" );
                    boost::sregex_iterator end;
                    for( boost::sregex_iterator it( currentLine.begin(), currentLine.end(), idRegex ); it != end; ++it )
                        for( unsigned int i = 1; i < it->size(); ++i )
                            if( ( *it )[ i ].matched )
                            {
                                const std::string matchId( (*it)[i].first, (*it)[i].second );
                                unsigned long curId =boost::lexical_cast< unsigned long >( matchId );
                                if( curId > maxId )
                                    maxId = curId;
                            }
                }
                ifile.close();
            }
        }
        catch( ... )
        {
            // NOTHING
        }
        return maxId;
    }

    unsigned long FindMaxId( const MIL_Config& config )
    {
        unsigned long maxUrbanId = FindMaxIdInFile( config.GetUrbanFile() );
        unsigned long maxOrbatId = FindMaxIdInFile( config.GetOrbatFile() );
        return std::max( maxUrbanId, maxOrbatId ) + 2;
    }
}

MIL_AgentServer* MIL_AgentServer::pTheAgentServer_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer constructor
// Created: DFT 02-02-28
// Last modified: JVT 03-09-24
//-----------------------------------------------------------------------------
MIL_AgentServer::MIL_AgentServer( MIL_Config& config )
    : nSimState_            ( eSimPaused )
    , config_               ( config )
    , settings_             ( new tools::ExerciseSettings() )
    , nTimeStepDuration_    ( 1 )
    , nTimeFactor_          ( 1 )
    , nCurrentTimeStep_     ( 1 )
    , nSimTime_             ( 0 )
    , nRealTime_            ( 0 )
    , lastStep_             ( clock() )
    , nextPause_            ( config_.GetPausedAtStartup() ? 1 : 0 )
    , waitTicks_            ( 0 )
    , waitLatency_          ( 0 )
    , pEffectManager_       ( new MIL_EffectManager() )
    , pEntityManager_       ( 0 )
    , pWorkspaceDIA_        ( 0 )
    , pMeteoDataManager_    ( 0 )
    , pPathFindManager_     ( 0 )
    , pCheckPointManager_   ( 0 )
    , pAgentServer_         ( 0 )
    , pUrbanCache_          ( new MIL_UrbanCache() )
    , pResourceTools_       ( new ResourceTools() )
    , pBurningCells_        ( new MIL_BurningCells() )
    , pResourceNetworkModel_( new resource::ResourceNetworkModel() )
    , pProcessMonitor_      ( new ProcessMonitor() )
    , pObjectFactory_       ( new MIL_ObjectFactory( config.IsLegacy() ) )
{
    MIL_Time_ABC::RegisterTime( *this );
    loopTimer_.Start();

    // These two lines are incredibly stupid but necessary as
    // MIL_AgentServer instantiate objects accessing to itself via its
    // singleton API. That is what you get with singletons and
    // lazyness...
    assert( !pTheAgentServer_ );
    pTheAgentServer_ = this;
    AgentServerInit initGuard( pTheAgentServer_ );

    MIL_IDManager::SetKeepIdsMode( true, FindMaxId( config_ ) );
    config_.AddFileToCRC( config_.GetExerciseFile() );
    config_.GetLoader().LoadFile( config_.GetSettingsFile(), boost::bind( &tools::ExerciseSettings::Load, settings_, _1 ) );
    ReadStaticData();
    pPathFindManager_ = new DEC_PathFind_Manager( config_, pObjectFactory_->GetMaxAvoidanceDistance(), pObjectFactory_->GetDangerousObjects() );
    if( config_.HasCheckpoint() )
    {
        MIL_CheckPointManager::LoadCheckPoint( config_, *pObjectFactory_ );
        pEntityManager_->Synchronize();
    }
    else
    {
        // $$$$ NLD 2007-01-11: A nettoyer - pb pEntityManager_ instancié par checkpoint
        pMeteoDataManager_ = new PHY_MeteoDataManager( config_ );
        pEntityManager_ = new MIL_EntityManager( *this, *pEffectManager_, *pObjectFactory_, config_ );
        pCheckPointManager_ = new MIL_CheckPointManager( config_ );
        pEntityManager_->ReadODB( config_ );
        pEntityManager_->LoadUrbanModel( config_ );
        pEntityManager_->Finalize();
    }
    MIL_IDManager::SetKeepIdsMode( false );
    Resume( nextPause_, 0, 0 );
    timerManager_.Register( *this );
    MT_LOG_INFO_MSG( "Tick duration : " << nTimeStepDuration_ << " seconds" );
    MT_LOG_INFO_MSG( "Acceleration factor : " << nTimeFactor_ );
    MT_LOG_INFO_MSG( "End tick : " << config_.GetEndTick() );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    MT_LOG_STARTUP_MESSAGE( "---- SIM Initialized ----" );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );

    // Must be the last action of the constructor
    initGuard.Done();
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer destructor
// Created: DFT 02-02-28
//-----------------------------------------------------------------------------
MIL_AgentServer::~MIL_AgentServer()
{
    MT_LOG_INFO_MSG( "Terminating Simulation..." );
    timerManager_.Unregister( *this );
    MT_LOG_INFO_MSG( "Terminating pathfind threads" );
    delete pPathFindManager_;
    delete pBurningCells_;
    // $$$$ AGE 2005-02-21:
//    MT_LOG_INFO_MSG( "Cleaning up simulation data" );
//    delete pEntityManager_;
//    delete pMeteoDataManager_;
//    MT_LOG_INFO_MSG( "Terminating DirectIA" );
//    delete pWorkspaceDIA_;
//    MT_LOG_INFO_MSG( "Terminating Network" );
//    delete pAgentServer_;
//    delete pCheckPointManager_;
    delete pProcessMonitor_;
    delete settings_;
    //    MT_LOG_INFO_MSG( "Terminating Terrain" );
//    TER_World::DestroyWorld();
    MIL_Time_ABC::UnregisterTime( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadStaticData
// Created: JVT 2005-03-07
// -----------------------------------------------------------------------------
void MIL_AgentServer::ReadStaticData()
{
    nTimeStepDuration_ = config_.GetTimeStep();
    nTimeFactor_ = config_.GetTimeFactor();
    ReadTerData();
    pWorkspaceDIA_ = new DEC_Workspace( config_ );
    PHY_MeteoDataManager::Initialize();
    MIL_EntityManager::Initialize( config_, *this, *pObjectFactory_ );
    pAgentServer_ = new NET_AgentServer( config_, *this );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::ReadTerData
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
void MIL_AgentServer::ReadTerData()
{
    MT_LOG_INFO_MSG( "Initializing terrain" );
    config_.AddFileToCRC( config_.GetTerrainFile() );
    MT_LOG_INFO_MSG( "Terrain: " << config_.GetTerrainFile() );
    TER_World::Initialize( config_ );
    MT_LOG_INFO_MSG( MT_FormatString( "Terrain size (w x h): %.2fkm x %.2fkm", TER_World::GetWorld().GetWidth() / 1000., TER_World::GetWorld().GetHeight()  / 1000. ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::Update
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
bool MIL_AgentServer::Update()
{
    timerManager_.Update();
    WaitForNextStep();
    return nSimState_ != MIL_AgentServer::eSimStopped;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::WaitForNextStep
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
void MIL_AgentServer::WaitForNextStep()
{
    assert( pAgentServer_ );
    pAgentServer_->Update();
    pPathFindManager_->Update();
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
    nSimTime_ += nTimeStepDuration_;
    nRealTime_ += nTimeStepDuration_;
    lastStep_ = clock();
    MainSimLoop();
    ++nCurrentTimeStep_;
    if( config_.GetEndTick() && nCurrentTimeStep_ > config_.GetEndTick() )
        nSimState_ = eSimStopped;

    if( nextPause_ > 0 && --nextPause_ == 0 )
        Pause( 0, 0 );
}

namespace
{
    double UpdatePathfind( DEC_PathFind_Manager& manager )
    {
        MT_Profiler profiler;
        profiler.Start();
        manager.UpdateInSimulationThread();
        return profiler.Stop();
    }
}

//-----------------------------------------------------------------------------
// Name:  MIL_AgentServer::mainSimLoop
// Created: JVT 02-06-21
// Last modified: JVT 03-04-03
//-----------------------------------------------------------------------------
void MIL_AgentServer::MainSimLoop()
{
    SendMsgBeginTick();
    GetUrbanCache().Clear();
    pEntityManager_->Update();
    pMeteoDataManager_->Update( nRealTime_ );
    pResourceNetworkModel_->Update();
    const double pathfindTime = UpdatePathfind( *pPathFindManager_ );
    SendMsgEndTick();
    pProcessMonitor_->MonitorProcess();
    const double lastTime = loopTimer_.Stop();
    loopTimer_.Start();
    MT_LOG_INFO_MSG( MT_FormatString( "**** Time tick %d %.2fms - Profiling (K/D/A/E/S) : %.2fms %.2fms (A:%.2f P:%.2f Pop:%.2f DEC:%.2f) %.2fms %.2fms %.2fms - Wait %.2fms %d ticks - PathFind : %d short %d long %d done %.2fms - Model : %d nodes - RAM : %.3f MB / %.3f MB (VM)",
        nCurrentTimeStep_, lastTime, pEntityManager_->GetKnowledgesTime(), pEntityManager_->GetDecisionsTime(),
        pEntityManager_->GetAutomatesDecisionTime(), pEntityManager_->GetPionsDecisionTime(), pEntityManager_->GetPopulationsDecisionTime(), sword::Brain::GetTotalTime(),
        pEntityManager_->GetActionsTime(), pEntityManager_->GetEffectsTime(), pEntityManager_->GetStatesTime(),
        profiler_.GetLastTime(), waitTicks_, pPathFindManager_->GetNbrShortRequests(), pPathFindManager_->GetNbrLongRequests(), pPathFindManager_->GetNbrTreatedRequests(), pathfindTime,
        pEntityManager_->GetModelCount(), pProcessMonitor_->GetMemory() / 1048576., pProcessMonitor_->GetVirtualMemory() / 1048576. ) );
    pEntityManager_->LogInfo();
    sword::Brain::ResetProfiling( config_.IsDecisionalProfilingEnabled() );
    pEntityManager_->Clean();
    pCheckPointManager_->Update();
    Wait();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendMsgBeginTick
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendMsgBeginTick() const
{
    client::ControlBeginTick msgBeginTick;
    msgBeginTick().set_current_tick( GetCurrentTimeStep() );
    NET_ASN_Tools::WriteGDH( nRealTime_, *msgBeginTick().mutable_date_time() );
    msgBeginTick.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendMsgEndTick
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendMsgEndTick() const
{
    assert( pProcessMonitor_ );
    client::ControlEndTick msgEndTick;
    msgEndTick().set_current_tick( GetCurrentTimeStep() );
    msgEndTick().set_tick_duration ( static_cast< int >( loopTimer_.GetLastTime() ) );
    msgEndTick().set_long_pathfinds( pPathFindManager_->GetNbrLongRequests() );
    msgEndTick().set_short_pathfinds( pPathFindManager_->GetNbrShortRequests() );
    msgEndTick().set_memory( static_cast< int >( pProcessMonitor_->GetMemory() ) );
    msgEndTick().set_virtual_memory( static_cast< int >( pProcessMonitor_->GetVirtualMemory() ) );
    msgEndTick.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendStateToNewClient
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendStateToNewClient() const
{
    pEntityManager_->SendStateToNewClient();
    pMeteoDataManager_->SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::save
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_AgentServer::save( MIL_CheckPointOutArchive& file ) const
{
    const std::string localTime = boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() );
    file << nTimeFactor_
         << nCurrentTimeStep_
         << nSimTime_
         << pMeteoDataManager_    // données statiques + météo locales gérées par MOS
         << pEntityManager_
//         << pWorkspaceDIA_        // uniquement données statiques
//         << timerManager_         // pas de données ( MIL_AgentServer::Initialize )
//         << pPathFindManager_     // pas stockés
         << pCheckPointManager_
//         << pAgentServer_         // moi-même ( static )
         << nInitialRealTime_
         << nRealTime_
         << localTime;
    pBurningCells_->save( file );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::load
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_AgentServer::load( MIL_CheckPointInArchive& file )
{
    file >> nTimeFactor_
         >> nCurrentTimeStep_
         >> nSimTime_
         >> pMeteoDataManager_
         >> pEntityManager_
//         >> pWorkspaceDIA_
//         >> timerManager_
//         >> pPathFindManager_
         >> pCheckPointManager_
//         >> pAgentServer_
         >> nInitialRealTime_
         >> nRealTime_
         >> localTime_;
    pBurningCells_->load( file );
    pBurningCells_->finalizeLoad( GetEntityManager() );
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
// Name: MIL_AgentServer::WriteKnowledges
// Created: NPT 2012-08-08
// -----------------------------------------------------------------------------
void MIL_AgentServer::WriteKnowledges( xml::xostream& xos ) const
{
    assert( pEntityManager_ );
    pEntityManager_->WriteKnowledges( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::WriteWeather
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_AgentServer::WriteWeather( xml::xostream& xos ) const
{
    GetWorkspace().GetMeteoDataManager().WriteWeather( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::WriteUrban
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_AgentServer::WriteUrban( xml::xostream& xos ) const
{
    assert( pEntityManager_ );
    pEntityManager_->WriteUrban( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SendControlInformation
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::SendControlInformation() const
{
    client::ControlInformation message;
    message().set_current_tick( GetCurrentTimeStep() );
    NET_ASN_Tools::WriteGDH( nInitialRealTime_, *message().mutable_initial_date_time() );
    NET_ASN_Tools::WriteGDH( nRealTime_, *message().mutable_date_time() );
    message().set_tick_duration( GetTickDuration() );
    message().set_time_factor( nTimeFactor_ );
    message().set_status( sword::EnumSimulationState( nSimState_ == eSimWait ? eSimRunning : nSimState_ ) );
    message().set_checkpoint_frequency( GetCheckPointManager().GetCheckPointFrequency() );
    message().set_send_vision_cones( false );
    message().set_profiling_enabled( config_.IsDecisionalProfilingEnabled() );
    if( !localTime_.empty() )
        message().mutable_checkpoint_real_time()->set_data( localTime_ );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Stop
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Stop( unsigned int nCtx, unsigned int clientId )
{
    client::ControlStopAck msg;
    if( nSimState_ == eSimStopped )
        msg().set_error_code( sword::ControlAck::error_not_started );
    else
    {
        nSimState_ = eSimStopped;
        MT_Timer_ABC::Stop();
        MT_LOG_INFO_MSG( "Simulation stopped" );
        msg().set_error_code( sword::ControlAck::no_error );
    }
    msg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Pause
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Pause( unsigned int nCtx, unsigned int clientId )
{
    client::ControlPauseAck msg;
    if( nSimState_ != eSimRunning && nSimState_ != eSimWait )
        msg().set_error_code( sword::ControlAck::error_already_paused );
    else
    {
        nSimState_ = eSimPaused;
        MT_Timer_ABC::Stop();
        MT_LOG_INFO_MSG( "Simulation paused" );
        msg().set_error_code( sword::ControlAck::no_error );
    }
    msg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Resume
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Resume( unsigned int ticks, unsigned int nCtx, unsigned int clientId )
{
    nextPause_ = ticks;
    client::ControlResumeAck msg;
    if( nSimState_ != eSimPaused )
        msg().set_error_code( sword::ControlAck::error_not_paused );
    else
    {
        nSimState_ = eSimRunning;
        MT_Timer_ABC::Start( static_cast< int >( 1000 * nTimeStepDuration_ / nTimeFactor_ ) );
        MT_LOG_INFO_MSG( "Simulation resumed" );
        msg().set_error_code( sword::ControlAck::no_error );
    }
    msg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Start
// Created: MCO 2012-11-14
// -----------------------------------------------------------------------------
void MIL_AgentServer::Start()
{
    SendControlInformation();
    Continue();
    waitLatency_ = config_.GetTickLatency();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Wait
// Created: MCO 2012-11-08
// -----------------------------------------------------------------------------
void MIL_AgentServer::Wait()
{
    ++waitTicks_;
    MT_LOG_DEBUG_MSG( "Simulation is " << waitTicks_ << " ticks ahead of dispatcher" );
    if( waitTicks_ > waitLatency_ )
    {
        MT_LOG_DEBUG_MSG( "Simulation starts waiting for dispatcher" );
        nSimState_ = eSimWait;
        MT_Timer_ABC::Wait();
        profiler_.Start();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Continue
// Created: JSR 2011-10-28
// -----------------------------------------------------------------------------
void MIL_AgentServer::Continue()
{
    --waitTicks_;
    MT_LOG_DEBUG_MSG( "Simulation is " << waitTicks_ << " ticks ahead of dispatcher" );
    if( waitTicks_ == waitLatency_ )
    {
        profiler_.Stop();
        MT_Timer_ABC::Continue();
        MT_LOG_DEBUG_MSG( "Simulation stops waiting for dispatcher" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetTimeFactor
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::SetTimeFactor( unsigned int timeFactor )
{
    client::ControlChangeTimeFactorAck msg;
    if( timeFactor == 0 )
        msg().set_error_code( sword::ControlAck::error_invalid_time_factor );
    else
    {
        nTimeFactor_ = timeFactor;
        if( nSimState_ == eSimRunning || nSimState_ == eSimWait )
        {
            MT_Timer_ABC::Start( static_cast< int >( 1000 * nTimeStepDuration_ / nTimeFactor_ ) );
            MT_LOG_INFO_MSG( "Time factor set to " << nTimeFactor_ );
        }
        msg().set_error_code( sword::ControlAck::no_error );
    }
    msg().set_time_factor( nTimeFactor_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetRealTime
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
void MIL_AgentServer::SetRealTime( const std::string& realTime )
{
    unsigned int secs = 0;
    sword::DateTime datetime;
    datetime.set_data( realTime );
    NET_ASN_Tools::ReadGDH( datetime, secs );
    client::ControlDateTimeChangeAck ack;
    if( secs < nInitialRealTime_ )
        ack().set_error_code( sword::ControlAck::error_invalid_date_time );
    else
    {
        nRealTime_ = secs;
        ack().set_error_code( sword::ControlAck::no_error );
    }
    ack.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::RealTimeToTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
unsigned int MIL_AgentServer::RealTimeToTick( unsigned int rt ) const
{
    return ( rt - nRealTime_ + nSimTime_ ) / nTimeStepDuration_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::TickToRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
unsigned int MIL_AgentServer::TickToRealTime( unsigned int tick ) const
{
    return nTimeStepDuration_ * tick - nSimTime_ + nRealTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetObjectFactory
// Created: LGY 2012-09-14
// -----------------------------------------------------------------------------
MIL_ObjectFactory& MIL_AgentServer::GetObjectFactory() const
{
    return *pObjectFactory_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::CreateWorkspace
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
void MIL_AgentServer::CreateWorkspace( MIL_Config& config )
{
    assert( pTheAgentServer_ == 0 );
    // MIL_AgentServer constructor sets pTheAgentServer_ to this
    new MIL_AgentServer( config );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::DestroyWorkspace
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
void MIL_AgentServer::DestroyWorkspace()
{
    if( pTheAgentServer_ )
    {
        delete pTheAgentServer_;
        pTheAgentServer_ = 0;
    }
}

