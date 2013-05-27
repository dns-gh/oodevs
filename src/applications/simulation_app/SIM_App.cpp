//*****************************************************************************
//
// $Created: NLD 2002-08-07 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_App.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 17:21 $
// $Revision: 62 $
// $Workfile: SIM_App.cpp $
//
//*****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_App.h"

#include "dispatcher/DispatcherLoader.h"
#include "FileLoaderObserver.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_Version.h"
#include "resource.h"
#include "SIM_NetworkLogger.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointManager.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "simulation_kernel/MIL_Random.h"
#include "simulation_kernel/Tools/MIL_Config.h"
#include "tools/ExerciseConfig.h"
#include "tools/IpcWatch.h"
#include "tools/Version.h"
#include "tools/WaitEvent.h"
#include "tools/WinArguments.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <xeumeuleu/xml.hpp>

#include <csignal>
#include <ctime>
#include <fcntl.h>
#include <io.h>

#define MY_WM_NOTIFYICON WM_USER + 1

static const int NUM_ICON_FOR_ANIMATION = 2;
static int IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1 };

// -----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
SIM_App::SIM_App( const std::wstring& cmdLine, int maxConnections, bool verbose )
    : maxConnections_( maxConnections )
    , verbose_       ( verbose )
    , observer_      ( new FileLoaderObserver() )
    , config_        ( new MIL_Config( *observer_ ) )
    , winArguments_  ( new tools::WinArguments( cmdLine ) )
    , quit_          ( new tools::WaitEvent() )
    , result_        ( EXIT_SUCCESS )
{
    config_->Parse( winArguments_->Argc(), const_cast< char** >( winArguments_->Argv() ) );

    bool bClearPreviousLog = !config_->HasCheckpoint();
    tools::ExerciseConfig* exerciceConfig = static_cast< tools::ExerciseConfig* >( config_.get() );
    logger_.reset( new MT_FileLogger( config_->BuildSessionChildFile( "Sim.log" ),
                                        exerciceConfig->GetSimLogFiles(), exerciceConfig->GetSimLogSize(),
                                        exerciceConfig->GetSimLogLevel(), bClearPreviousLog, MT_Logger_ABC::eSimulation, exerciceConfig->IsSimLogInBytes() ) );
    console_.reset( new MT_ConsoleLogger() );
    MT_LOG_REGISTER_LOGGER( *console_ );
    MT_LOG_REGISTER_LOGGER( *logger_ );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( ( "Sword(tm) Simulation - Version " + std::string( tools::AppVersion() ) + " - " MT_COMPILE_TYPE + ( config_->IsLegacy() ? " - Legacy mode" : "" ) ).c_str() );
    MT_LOG_STARTUP_MESSAGE( ( "Starting simulation - " + boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) ).c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    if( config_->UseNetworkLogger() )
    {
        try
        {
            pNetworkLogger_.reset( new SIM_NetworkLogger( config_->GetNetworkLoggerPort(), MT_Logger_ABC::eLogLevel_All ) );
            MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
        }
        catch( const std::exception& e )
        {
            MT_LOG_WARNING_MSG( "Network logger (telnet) not registered - Reason : '" << tools::GetExceptionMsg( e ) << "'" );
            pNetworkLogger_.reset();
        }
    }
}

//-----------------------------------------------------------------------------
// Name: SIM_App destructor
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
SIM_App::~SIM_App()
{
    quit_->Signal();
    if( dispatcher_.get() )
        dispatcher_->join();
    MIL_AgentServer::DestroyWorkspace();
    MT_LOG_UNREGISTER_LOGGER( *console_ );
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
    if( pNetworkLogger_.get() )
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
}

namespace
{

void RunDispatcher( const tools::WinArguments* winArguments, int maxConnections,
                    tools::WaitEvent* quit, int& result )
{
    try
    {
        dispatcher::DispatcherLoader loader( winArguments->Argc(),
            const_cast< char** >( winArguments->Argv() ), maxConnections );
        while( !quit->IsSignaled() )
        {
            if( !loader.Update() )
                throw MASA_EXCEPTION( "An error occured in dispatcher, see dispatcher.log for more information" );
            quit->Wait( boost::posix_time::milliseconds( 25 ) );
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "dispatcher: " << tools::GetExceptionMsg( e ) );
        result = EXIT_FAILURE;
    }
    quit->Signal();
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Initialize()
{
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    if( config_->IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        dispatcher_.reset( new boost::thread( boost::bind( &RunDispatcher,
                           winArguments_.get(), maxConnections_, quit_.get(), boost::ref( result_ ) ) ) );
    }
    MIL_Random::Initialize( config_->GetRandomSeed(), config_->GetRandomGaussian(), config_->GetRandomDeviation(), config_->GetRandomMean() );
    MIL_AgentServer::CreateWorkspace( *config_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    tools::ipc::Watch watch( *quit_ );
    while( !quit_->IsSignaled() )
        if( !MIL_AgentServer::GetWorkspace().Update() )
            break;
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    Initialize();
    Run();
    return result_;
}
