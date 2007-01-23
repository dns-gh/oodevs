
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
#include <ctime>
#include <signal.h>
#include "SIM_NetworkLogger.h"
#include "SIM_Dispatcher.h"

#include "simulation_kernel/MIL_AgentServer.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

bool SIM_App::bCrashWithCoreDump_ = false;
bool SIM_App::bUserInterrupt_     = false;

//-----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: NLD 2002-08-07
// Last modified: JVT 03-07-24
//-----------------------------------------------------------------------------
SIM_App::SIM_App( int argc, char** argv )
    : startupConfig_ ( argc, argv )
    , pNetworkLogger_( 0 )
    , pDispatcher_   ( 0 )
{
    std::string strMsg = "Scipio SIM - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( strMsg.c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    if( startupConfig_.UseNetworkLogger() )
    {
        try
        {
            pNetworkLogger_ = new SIM_NetworkLogger( startupConfig_.GetNetworkLoggerPort() );
            MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
        }
        catch( MT_Exception& exception )
        {
            MT_LOG_WARNING_MSG( MT_FormatString( "Network logger (telnet) not registered - Reason : '%s'", exception.GetInfo().c_str() ) );
            pNetworkLogger_ = 0;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: SIM_App destructor
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
SIM_App::~SIM_App()
{
    if( pNetworkLogger_ )
    {
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
        delete pNetworkLogger_;
    }

//    if( pDispatcher_ )
//        delete pDispatcher_;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::ConsoleEventHandler
// Created: NLD 2006-06-22
// -----------------------------------------------------------------------------
bool SIM_App::ConsoleEventHandler( int nEvent )
{
    switch( nEvent )
    {
        case CTRL_C_EVENT       : MT_LOG_INFO_MSG( "Ctrl+C event ignored" );     return true;
        case CTRL_BREAK_EVENT   : MT_LOG_INFO_MSG( "Ctrl+Break event ignored" ); return true;
        case CTRL_CLOSE_EVENT   : MT_LOG_INFO_MSG( "Close event" );              bUserInterrupt_ = true; return false;
        case CTRL_LOGOFF_EVENT  : MT_LOG_INFO_MSG( "Logoff event" );             bUserInterrupt_ = true; return false;
        case CTRL_SHUTDOWN_EVENT: MT_LOG_INFO_MSG( "Shutdown event" );           bUserInterrupt_ = true; return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Initialize()
{
#if !defined( _DEBUG ) 
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleEventHandler, TRUE );
#endif

    if( startupConfig_.IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        pDispatcher_ = new SIM_Dispatcher( startupConfig_ );
    }

    MT_Profiler::Initialize();

    MIL_AgentServer::CreateWorkspace( startupConfig_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Cleanup
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Cleanup()
{
    MIL_AgentServer::DestroyWorkspace();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    bool bRun = true;
    while( bRun && !bUserInterrupt_ )
    {            
        MIL_AgentServer::E_SimState nSimState = MIL_AgentServer::GetWorkspace().Update(); 
        bRun = ( nSimState != MIL_AgentServer::eSimStopped );

        if( pNetworkLogger_ )
            pNetworkLogger_->Update();
        ::Sleep( 0 );
    }
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    if( startupConfig_.IsTestMode() )
        return Test();

    Initialize();
    Run       ();
    Cleanup   ();
    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Test
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
int SIM_App::Test()
{
    static const std::string prefix( "ERROR: " );
    try
    {
        Initialize();
        Cleanup   ();
        return EXIT_SUCCESS;
    }
    catch( MT_ScipioException& exception )
    {
        std::cerr << Wrap( exception.GetMsg(), prefix ) << std::endl
                  << Wrap( exception.GetDescription(), prefix ) << std::endl;
    }
    catch( MT_Exception& exception )
    {
        std::cerr << Wrap( exception.GetInfo(), prefix ) << std::endl;
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    catch( std::bad_alloc& /*exception*/ )
    {
        std::cerr << Wrap( "Allocation error : not enough memory", prefix )  << std::endl;
    }
    catch( std::exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::IsAlreadyWrapped
// Created: MCO 2005-02-22
// -----------------------------------------------------------------------------
bool SIM_App::IsAlreadyWrapped( const std::string& content ) const
{
    return content.find( "WARNING" ) != std::string::npos || content.find( "ERROR" ) != std::string::npos || content.find( "INFO" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Wrap
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
std::string SIM_App::Wrap( const std::string& content, const std::string& prefix ) const
{
    std::string result;
    std::stringstream input( content );
    std::string line;
    bool bFirst = true;
    while( std::getline( input, line ) )
    {
        if( ! bFirst )
            result += '\n';
        else
            bFirst = false;
        if( ! IsAlreadyWrapped( line ) )
            result += prefix;
        result += line;
    }
    return result;
}
