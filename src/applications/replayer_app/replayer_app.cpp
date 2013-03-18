// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/Path.h"
#include <tools/Exception.h>
#include "tools/WinArguments.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <windows.h>

int Run( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    tools::WinArguments winArgs( lpCmdLine );
    tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    MT_CrashHandler::SetRootDirectory( debugDir );

    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger        consoleLogger;
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
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}

int WINAPI wWinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
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
