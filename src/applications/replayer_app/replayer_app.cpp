// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/IpcWatch.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Exception.h>
#include <tools/win32/CrashHandler.h>
#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <windows.h>

namespace
{

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

} // namespace

int main( int /*argc*/, char* /*argv*/[] )
{
    tools::WinArguments winArgs( GetCommandLineW() );
    tools::InitPureCallHandler();
    boost::scoped_ptr< MT_FileLogger > fileLogger;
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    fileLogger.reset( new MT_FileLogger( debugDir / "replayer.log", 1, 0,
        MT_Logger_ABC::eLogLevel_All ) );
    MT_LOG_REGISTER_LOGGER( *fileLogger );

    try
    {
        debugDir.CreateDirectories();
        MT_CrashHandler::SetRootDirectory( debugDir );
        tools::InitCrashHandler( &CrashHandler );
    }
    catch( std::exception& )
    {
        // NOTHING
    }

    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    try
    {
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( "sword-replayer", !winArgs.HasOption( "verbose" ) );
#endif
        tools::SetCodec();
        HINSTANCE hInstance = GetModuleHandle( NULL );
        HINSTANCE prevInstance = GetModuleHandle( NULL );
        App app( hInstance, prevInstance, GetCommandLineW(), 0, !winArgs.HasOption( "--no-log" ) );
        app.Execute();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        nResult = EXIT_FAILURE;
    }
    if( fileLogger )
        MT_LOG_UNREGISTER_LOGGER( *fileLogger );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
