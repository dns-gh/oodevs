// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Exception.h>
#include <windows.h>

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( LPWSTR lpCmdLine )
{
    // Init logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int maxConnections = 1;
    int nResult = EXIT_FAILURE;
    try
    {
        tools::WinArguments winArgs( lpCmdLine );
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        // verbose mode
        const bool verbose = winArgs.HasOption( "--verbose" );
        // Check license
        license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
        license_gui::LicenseDialog::CheckLicense( "sword-dispatcher", !verbose, &maxConnections );
#endif

        // Execute dispatcher
        tools::SetCodec();
        Application app( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ), maxConnections );
        nResult = app.Execute( winArgs.HasOption( "--test" ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}

//-----------------------------------------------------------------------------
// Name: main constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE /*hinstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR lpCmdLine, int /*nCmdShow*/ )
{
    __try
    {
        return Run( lpCmdLine );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
    return 0;
}
