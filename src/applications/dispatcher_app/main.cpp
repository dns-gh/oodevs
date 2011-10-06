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
#include "tools/WinArguments.h"
#include "tools/win32/FlexLm.h"
#include <windows.h>
#pragma warning( push )
#pragma warning( disable: 4512 )
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
        std::auto_ptr< FlexLmLicense > license_runtime( FlexLmLicense::CheckLicense( "sword-runtime", 1.0f, silentMode ) );
        std::auto_ptr< FlexLmLicense > license_dispatch( FlexLmLicense::CheckLicense( "sword-dispatcher", 1.0f, silentMode ) );
        try
        {
            maxConnections = license_dispatch->GetAuthorisedUsers();
        }
        catch( FlexLmLicense::LicenseError& )
        {
            maxConnections = 1;
        }
#endif

        // Execute dispatcher
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
    /*__try
    {
        return Run( argc, argv );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }*/
    try
    {
        return Run( lpCmdLine );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
    return 0;
}
