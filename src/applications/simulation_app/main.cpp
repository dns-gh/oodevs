#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "SIM_NetworkLogger.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/VersionHelper.h"
#include "tools/WinArguments.h"
#include "simulation_terrain/TER_Logger.h"
#include <license_gui/LicenseDialog.h>
#include <tools/win32/FlexLm.h>
#include <tools/win32/CrashHandler.h>
#include <masalloc/masalloc.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <new.h>
#include <direct.h>

namespace
{

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

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

}  // namespace

//-----------------------------------------------------------------------------
// Name: SetLowFragmentationHeapAlgorithm
// Created: NLD 2006-01-20
// -----------------------------------------------------------------------------
void SetLowFragmentationHeapAlgorithm()
{
    ULONG ulHeapCompatibilityInformation = 2;
    HeapSetInformation( GetProcessHeap(), HeapCompatibilityInformation, &ulHeapCompatibilityInformation, sizeof(ulHeapCompatibilityInformation) );
}

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int main( int /*argc*/, char* /*argv*/[] )
{
    tools::WinArguments winArgs( GetCommandLineW() );
    tools::InitPureCallHandler();

    // Init logger & crash handler
    boost::scoped_ptr< MT_FileLogger > fileLogger;
    boost::scoped_ptr< MT_FileLogger > crashFileLogger;
    MT_ConsoleLogger consoleLogger;
    try
    {
        tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
        debugDir.CreateDirectories();

        MT_CrashHandler::SetRootDirectory( debugDir );
        tools::InitCrashHandler( &CrashHandler );

        fileLogger.reset( new MT_FileLogger( debugDir / "Sim.log", 1, -1, MT_Logger_ABC::eLogLevel_All ) );
        const tools::Path filename = debugDir / "Crash Version " + tools::AppProjectVersion() + ".log";
        crashFileLogger.reset( new MT_FileLogger( filename, 1, -1,  MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError ) );
        MT_LOG_REGISTER_LOGGER( *fileLogger );
        MT_LOG_REGISTER_LOGGER( *crashFileLogger );
        MT_LOG_REGISTER_LOGGER( consoleLogger );
        InitializeTerrainLogger();
    }
    catch( std::exception& )
    {
        // NOTHING
    }

    boost::scoped_ptr< SIM_App > app;
    int nResult = EXIT_FAILURE;
    int maxConnections = 1;
    // verbose mode
    bool verbose = winArgs.HasOption( "--verbose" ) || winArgs.HasOption( "--verbose=true" ) ;
    try
    {
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
        HINSTANCE hInstance = GetModuleHandle( NULL );
        HINSTANCE prevInstance = GetModuleHandle( NULL );
        app.reset( new SIM_App( hInstance, prevInstance, GetCommandLineW(), 0, maxConnections, verbose ) );
        MT_LOG_UNREGISTER_LOGGER( *fileLogger );
        nResult = app->Execute();
    }
    catch( const FlexLmLicense::LicenseError& e )
    {
        MT_LOG_ERROR_MSG( e.CreateLoggerMsg() );
    }
    catch( const xml::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
    google::protobuf::ShutdownProtobufLibrary();
    app.reset();
    MT_LOG_UNREGISTER_LOGGER( *crashFileLogger );
    MT_LOG_UNREGISTER_LOGGER( *fileLogger );
    return nResult;
}
