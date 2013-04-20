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
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Exception.h>
#include <tools/Path.h>
#include <tools/win32/CrashHandler.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
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

int WINAPI wWinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    tools::WinArguments winArgs( lpCmdLine );
    tools::InitPureCallHandler();
    boost::scoped_ptr< MT_FileLogger > fileLogger;
    tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "" ));
    if( !debugDir.IsEmpty() )
    {
        debugDir.CreateDirectories();
        fileLogger.reset( new MT_FileLogger( debugDir / "replayer.log", 1, -1,
            MT_Logger_ABC::eLogLevel_All ) );
        MT_LOG_REGISTER_LOGGER( *fileLogger );
    }
    else
    {
        debugDir = tools::Path::FromUTF8( "./Debug" );
    }

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
        App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow, !winArgs.HasOption( "--no-log" ) );
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
