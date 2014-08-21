//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentServer.h"
#include "ActionManager.h"
#include "PathfindComputer.h"

#include "CheckPoints/MIL_CheckPointManager.h"
#include "CheckPoints/SerializationTools.h"
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
#include "protocol/ClientSenders.h"
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
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/algorithm/string/regex.hpp>

namespace bpt = boost::posix_time;

unsigned long FindMaxIdInFile( const tools::Path& filePath )
{
    static const boost::regex idRegex( "id=\"([0-9]+)\"" );
    unsigned long maxId = 0;
    std::string line;
    tools::Ifstream ifile( filePath );
    while( std::getline( ifile, line ) )
    {
        boost::sregex_iterator it( line.begin(), line.end(), idRegex );
        const boost::sregex_iterator end;
        for( ; it != end; ++it )
            for( int i = 1; i < static_cast< int >( it->size() ); ++i )
                maxId = std::max( maxId,
                    boost::lexical_cast< unsigned long >( it->str( i ) ));
    }
    return maxId;
}

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

    unsigned long FindMaxId( const MIL_Config& config )
    {
        unsigned long maxUrbanId = FindMaxIdInFile( config.GetUrbanFile() );
        unsigned long maxOrbatId = FindMaxIdInFile( config.GetOrbatFile() );
        return std::max( maxUrbanId, maxOrbatId ) + 2;
    }

    PHY_MeteoDataManager* CreateMeteoManager(
        const boost::shared_ptr< TER_World >& world, MIL_Config& config,
        uint32_t tickDuration )
    {
        auto xis = config.GetLoader().LoadFile( config.GetWeatherFile() );

        // Extract and configure exercise start time
        std::string date;
        *xis >> xml::start( "weather" )
                >> xml::start( "exercise-date" )
                    >> xml::attribute( "value", date )
                >> xml::end
            >> xml::end;
        const auto since = bpt::from_iso_string( date ) - bpt::from_time_t( 0 );
        MIL_AgentServer::GetWorkspace().SetInitialRealTime( since.total_seconds() );
        const auto now = MIL_Time_ABC::GetTime().GetRealTime();

        return new PHY_MeteoDataManager(
                world, *xis, config.GetDetectionFile(), now, tickDuration );
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
    , pObjectFactory_       ( new MIL_ObjectFactory() )
    , pathfindTime_         ( 0 )
    , updateState_          ( false )
    , actions_              ( new ActionManager() )
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
    config_.GetLoader().LoadFile( config_.GetSettingsFile(), boost::bind( &tools::ExerciseSettings::Load, settings_, _1 ) );

    nTimeStepDuration_ = config_.GetTimeStep();
    nTimeFactor_ = config_.GetTimeFactor();

    MT_LOG_INFO_MSG( "Initializing terrain" );
    MT_LOG_INFO_MSG( "Terrain: " << config_.GetTerrainFile() );
    TER_World::Initialize( config_ );
    MT_LOG_INFO_MSG( MT_FormatString( "Terrain size (w x h): %.2fkm x %.2fkm",
        TER_World::GetWorld().GetWidth() / 1000.,
        TER_World::GetWorld().GetHeight()  / 1000. ) );
    const auto world = TER_World::GetWorldPtr();

    pWorkspaceDIA_ = new DEC_Workspace( config_ );
    MIL_EntityManager::Initialize( config_.GetPhyLoader(), *this, *pObjectFactory_ );
    
    pPathFindManager_ = new DEC_PathFind_Manager( config_, pObjectFactory_->GetMaxAvoidanceDistance(), pObjectFactory_->GetDangerousObjects() );
    if( config_.HasCheckpoint() )
    {
        updateState_ = !config_.GetPausedAtStartup();
        MIL_CheckPointManager::LoadCheckPoint( config_, *pObjectFactory_, world );
        // pAgentServer_ must be initialised after LoadCheckpoint because actions_ is recreated during Load
        pAgentServer_ = new NET_AgentServer( config_, *this, *actions_ );
    }
    else
    {
        // $$$$ NLD 2007-01-11: A nettoyer - pb pEntityManager_ instancié par checkpoint
        pAgentServer_ = new NET_AgentServer( config_, *this, *actions_ );
        pMeteoDataManager_ = CreateMeteoManager( world, config, GetTickDuration() );
        pEntityManager_ = new MIL_EntityManager( *this, *pEffectManager_, *pObjectFactory_,
                config_, world, *actions_ );
        pCheckPointManager_ = new MIL_CheckPointManager( config_, world );
        pEntityManager_->ReadODB( config_ );
        pEntityManager_->LoadUrbanModel( config_ );
        pEntityManager_->Finalize();
        pathfinds_.reset( new PathfindComputer( *pPathFindManager_, TER_World::GetWorld() ) );
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
    pathfindTime_ += pPathFindManager_->Update();
    pathfinds_->Update( *actions_ );
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
    if( updateState_ )
        MainSimLoop();
    else
    {
        updateState_ = true;
        Wait();
    }
    ++nCurrentTimeStep_;
    if( config_.GetEndTick() && nCurrentTimeStep_ > config_.GetEndTick() )
        nSimState_ = eSimStopped;

    if( nextPause_ > 0 && --nextPause_ == 0 )
        Pause( 0, 0 );
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
    pPathFindManager_->UpdateInSimulationThread();
    SendMsgEndTick();
    pProcessMonitor_->MonitorProcess();
    const double lastTime = loopTimer_.Stop();
    loopTimer_.Start();
    MT_LOG_INFO_MSG( MT_FormatString( "**** Time tick %d %.2fms - Profiling (K/D/A/E/S) : %.2fms %.2fms (A:%.2f P:%.2f Pop:%.2f DEC:%.2f) %.2fms %.2fms %.2fms - Wait %.2fms %d ticks - PathFind : %d short %d long %d done %.2fms - RAM : %.3f MB / %.3f MB (VM)",
        nCurrentTimeStep_, lastTime, pEntityManager_->GetKnowledgesTime(), pEntityManager_->GetDecisionsTime(),
        pEntityManager_->GetAutomatesDecisionTime(), pEntityManager_->GetPionsDecisionTime(), pEntityManager_->GetPopulationsDecisionTime(), sword::Brain::GetTotalTime(),
        pEntityManager_->GetActionsTime(), pEntityManager_->GetEffectsTime(), pEntityManager_->GetStatesTime(),
        profiler_.GetLastTime(), waitTicks_, pPathFindManager_->GetNbrShortRequests(), pPathFindManager_->GetNbrLongRequests(), pPathFindManager_->GetNbrTreatedRequests(), pathfindTime_,
        pProcessMonitor_->GetMemory() / 1048576., pProcessMonitor_->GetVirtualMemory() / 1048576. ) );
    pathfindTime_ = 0;
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
    const std::string localTime = boost::posix_time::to_iso_string( boost::posix_time::second_clock::universal_time() );
    msgBeginTick().mutable_real_date_time()->set_data( localTime );
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
    actions_->SendStateToNewClient();
    pathfinds_->SendStateToNewClient();
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
         << localTime
         << actions_
         << pathfinds_;
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
         >> localTime_
         >> actions_
         >> pathfinds_;
    pBurningCells_->load( file );
    pBurningCells_->finalizeLoad( *pEntityManager_ );
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
    const uint32_t now = MIL_Time_ABC::GetTime().GetRealTime();
    GetWorkspace().GetMeteoDataManager().WriteWeather( xos, now );
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
    nSimState_ = eSimPaused;
    MT_Timer_ABC::Stop();
    MT_LOG_INFO_MSG( "Simulation paused" );

    client::ControlPauseAck msg;
    msg().set_current_tick( GetCurrentTimeStep() );
    msg().set_error_code( sword::ControlAck::no_error );
    msg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::Resume
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
void MIL_AgentServer::Resume( unsigned int ticks, unsigned int nCtx, unsigned int clientId )
{
    nextPause_ = ticks;
    nSimState_ = eSimRunning;
    MT_Timer_ABC::Start( static_cast< int >( 1000 * nTimeStepDuration_ / nTimeFactor_ ) );
    MT_LOG_INFO_MSG( "Simulation resumed" );

    client::ControlResumeAck msg;
    msg().set_current_tick( GetCurrentTimeStep() );
    msg().set_delay( ticks );
    msg().set_error_code( sword::ControlAck::no_error );
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
void MIL_AgentServer::SetRealTime( const std::string& realTime, unsigned int nCtx, unsigned int clientId )
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
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetConfig
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config& MIL_AgentServer::GetConfig()
{
    return config_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetWorkspaceDIA
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
DEC_Workspace& MIL_AgentServer::GetWorkspaceDIA() const
{
    assert( pWorkspaceDIA_ );
    return *pWorkspaceDIA_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetAgentServer
// Created: JVT 2002-07-17
//-----------------------------------------------------------------------------
NET_AgentServer& MIL_AgentServer::GetAgentServer() const
{
    assert( pAgentServer_ );
    return *pAgentServer_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetMeteoDataManager
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager& MIL_AgentServer::GetMeteoDataManager() const
{
    assert( pMeteoDataManager_ );
    return *pMeteoDataManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetPathFindManager
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
DEC_PathFind_Manager& MIL_AgentServer::GetPathFindManager() const
{
    assert( pPathFindManager_ );
    return *pPathFindManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCheckPointManager
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MIL_CheckPointManager& MIL_AgentServer::GetCheckPointManager() const
{
    assert( pCheckPointManager_ );
    return *pCheckPointManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSimTime
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
unsigned int MIL_AgentServer::GetSimTime() const
{
    return nSimTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetInitialRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void MIL_AgentServer::SetInitialRealTime( unsigned int time )
{
    nRealTime_ = nInitialRealTime_ = time;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetRealTime
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
unsigned int MIL_AgentServer::GetRealTime() const
{
    return nRealTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCurrentTimeStep
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
unsigned int MIL_AgentServer::GetCurrentTimeStep() const
{
    return nCurrentTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTickDuration
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
unsigned int MIL_AgentServer::GetTickDuration() const
{
    return nTimeStepDuration_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetEntityManager
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_EntityManager& MIL_AgentServer::GetEntityManager() const
{
    assert( pEntityManager_ );
    return *pEntityManager_;
}

MIL_EntityManager_ABC& MIL_AgentServer::GetEntities() const
{
    assert( pEntityManager_ );
    return *pEntityManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetUrbanCache
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
MIL_UrbanCache& MIL_AgentServer::GetUrbanCache() const
{
    assert( pUrbanCache_ );
    return *pUrbanCache_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetResourceNetworkModel
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
resource::ResourceNetworkModel& MIL_AgentServer::GetResourceNetworkModel() const
{
    assert( pResourceNetworkModel_ );
    return *pResourceNetworkModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetResourceTools
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
resource::ResourceTools_ABC& MIL_AgentServer::GetResourceTools() const
{
    assert( pResourceTools_ );
    return *pResourceTools_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetBurningCells
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
MIL_BurningCells& MIL_AgentServer::GetBurningCells() const
{
    assert( pBurningCells_ );
    return *pBurningCells_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetWorkspace
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
MIL_AgentServer& MIL_AgentServer::GetWorkspace()
{
    assert( pTheAgentServer_ );
    return *pTheAgentServer_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::IsInitialized
// Created: JSR 2010-11-22
// -----------------------------------------------------------------------------
bool MIL_AgentServer::IsInitialized()
{
    return pTheAgentServer_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSettings
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
tools::ExerciseSettings& MIL_AgentServer::GetSettings() const
{
    assert( settings_ != 0 );
    return *settings_;
}

PathfindComputer& MIL_AgentServer::GetPathfindComputer() const
{
    return *pathfinds_;
}
