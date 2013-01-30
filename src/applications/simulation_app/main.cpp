#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "SIM_NetworkLogger.h"
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Version.h"
#include <tools/win32/FlexLm.h>
#include <masalloc/masalloc.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <new.h>
#include <dbghelp.h>
#include <direct.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#pragma warning( pop )

int __cdecl NoMoreMemoryHandler( std::size_t nSize )
{
    int nResult = MessageBox( 0, MT_FormatString( "No more memory (%d bytes requested) - Retry ?", nSize ).c_str(), "SWORD - Memory error", MB_ICONERROR | MB_RETRYCANCEL | MB_TOPMOST );
    switch( nResult )
    {
    case IDRETRY:
        return 1;
    case IDCANCEL:
    default:
        throw std::bad_alloc();
    }
}

int __cdecl SilentNoMoreMemoryHandler( std::size_t /*nSize*/ )
{
    throw std::bad_alloc();
}

//-----------------------------------------------------------------------------
// Name: SetLowFragmentationHeapAlgorithm
// Created: NLD 2006-01-20
// -----------------------------------------------------------------------------
void SetLowFragmentationHeapAlgorithm()
{
    ULONG ulHeapCompatibilityInformation = 2;
    HeapSetInformation( GetProcessHeap(), HeapCompatibilityInformation, &ulHeapCompatibilityInformation, sizeof(ulHeapCompatibilityInformation) );
}

namespace
{
typedef std::vector< std::string > T_Args;
boost::optional< std::string > GetOption( const T_Args& args, const std::string& name )
{
    T_Args::const_iterator it = std::find( args.begin(), args.end(), name );
    if( it != args.end() )
        if( ++it != args.end() )
            return *it;
    return boost::none;
}
}

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    const T_Args argv = boost::program_options:: split_winmain( lpCmdLine );
    boost::optional< std::string > opt = GetOption( argv, "--debug-dir" );
    const std::string debugRoot = opt == boost::none ? "./Debug" : *opt;

    MT_CrashHandler::SetRootDirectory( debugRoot );

    // Init logger
    _mkdir( debugRoot.c_str() );
    MT_FileLogger           fileLogger     ( ( debugRoot + "/Sim.log" ).c_str(), 1, -1, MT_Logger_ABC::eLogLevel_All );
    const std::string filename = debugRoot + "/Crash Version " + std::string( tools::AppProjectVersion() ) + ".log";
    MT_FileLogger           crashFileLogger( filename.c_str(), 1, -1,  MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError );
    MT_LOG_REGISTER_LOGGER( fileLogger );
    MT_LOG_REGISTER_LOGGER( crashFileLogger );

    SIM_App* app = 0;
    int nResult = EXIT_FAILURE;
    bool verbose = false;
    int maxConnections = 1;
    try
    {
        // verbose mode
        verbose = std::find( argv.begin(), argv.end(), "--verbose" ) != argv.end();

        // Check license
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
        try
        {
            FlexLmLicense license_dispatch( "sword-dispatcher", 1.0f );
            maxConnections = license_dispatch.GetAuthorisedUsers();
        }
        catch( const FlexLmLicense::LicenseError& )
        {
            maxConnections = 1;
        }
#endif

        // Fragmentation heap
        SetLowFragmentationHeapAlgorithm();

        // Float exceptions
        unsigned int control_word;
        _controlfp_s( &control_word, _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT | _EM_INVALID, _MCW_EM ); // Exception raised for _EM_DENORMAL and _EM_ZERODIVIDE

        // Memory handlers
        _set_new_mode   ( 1 );
        _set_new_handler( verbose ? NoMoreMemoryHandler : SilentNoMoreMemoryHandler );

        // Execute simulation
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        app = new SIM_App( hinstance, hPrevInstance, lpCmdLine, nCmdShow, maxConnections, verbose );
        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        app->Execute();
        nResult = EXIT_SUCCESS;
    }
    catch( const FlexLmLicense::LicenseError& e )
    {
        MT_LOG_ERROR_MSG( e.CreateLoggerMsg() );
    }
    catch( const tools::Exception& e )
    {
        std::string errorMsg = e.CreateLoggerMsg();
        MT_LOG_ERROR_MSG( errorMsg );
        if( verbose )
            MessageBox( 0, errorMsg.c_str(), "SWORD - Internal exception", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( const xml::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        if( verbose )
            MessageBox( 0, tools::GetExceptionMsg( e ).c_str(), "SWORD - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( const std::bad_alloc& /*e*/ )
    {
        MT_LOG_ERROR_MSG( "Bad alloc" );
        if( verbose )
            MessageBox( 0, "Allocation error : not enough memory", "Simulation - Memory error", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        if( verbose )
            MessageBox( 0, tools::GetExceptionMsg( e ).c_str(), "SWORD - Exception standard", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    google::protobuf::ShutdownProtobufLibrary();
    delete app;
    MT_LOG_UNREGISTER_LOGGER( crashFileLogger );
    MT_LOG_UNREGISTER_LOGGER( fileLogger );
    return nResult;
}

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    __try
    {
        return Run( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
    return EXIT_FAILURE;
}
