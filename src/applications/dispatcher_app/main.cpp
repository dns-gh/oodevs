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

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( LPSTR lpCmdLine )
{
    int maxConnections = 10;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword-runtime", 1.0f ) );
    try
    {
        std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword-dispatcher", 1.0f ) );
        maxConnections = license->GetAuthorisedUsers();
    }
    catch( FlexLmLicense::LicenseError& /*error*/ )
    {
        maxConnections = 1;
    }
#endif

    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int nResult = 0;
    try
    {
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
