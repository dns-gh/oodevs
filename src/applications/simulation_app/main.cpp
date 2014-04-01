#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/VersionHelper.h"
#include "tools/WinArguments.h"
#include "simulation_terrain/TER_Logger.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Path.h>
#include <tools/StackContext.h>
#include <tools/win32/FlexLm.h>
#include <tools/win32/CrashHandler.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <new.h>
#include <direct.h>

namespace
{

tools::StackContext context;

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

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

        fileLogger.reset( new MT_FileLogger( debugDir / "Sim.log", 1, 0, MT_Logger_ABC::eLogLevel_All ) );
        const tools::Path filename = debugDir / "Crash Version " + tools::AppProjectVersion() + ".log";
        crashFileLogger.reset( new MT_FileLogger( filename, 1, 0, MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError ) );
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

    // verbose mode
    bool verbose = winArgs.HasOption( "--verbose" ) || winArgs.HasOption( "--verbose=true" ) ;
    try
    {
        int maxConnections = 0;
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
    catch( const tools::Exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetStackTraceAndMessage( e ));
    }
    google::protobuf::ShutdownProtobufLibrary();
    app.reset();
    MT_LOG_UNREGISTER_LOGGER( *crashFileLogger );
    MT_LOG_UNREGISTER_LOGGER( *fileLogger );
    return nResult;
}
