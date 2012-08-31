// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/WinArguments.h"
#include <windows.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( LPSTR lpCmdLine )
{
    // Init logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    bool silentMode = false;
    int maxConnections = 10;
    int nResult = EXIT_FAILURE;
    try
    {
        // Silent mode
        std::vector< std::string > argv = boost::program_options:: split_winmain( lpCmdLine );
        silentMode = ( std::find( argv.begin(), argv.end(), "--silent" ) != argv.end() );

#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        // Check license
        license_gui::LicenseDialog::CheckLicense( "sword-runtime", silentMode );
        license_gui::LicenseDialog::CheckLicense( "sword-dispatcher", silentMode, &maxConnections );
#endif

        // Execute dispatcher
        tools::SetCodec();
        tools::WinArguments winArgs( lpCmdLine );
        Application app( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ), maxConnections );
        nResult = app.Execute();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}

//-----------------------------------------------------------------------------
// Name: main constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE /*hinstance*/, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nCmdShow*/ )
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
