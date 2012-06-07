// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
int Run( int argc, char** argv )
{
    // Init logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int maxConnections = 0;
    int nResult = EXIT_FAILURE;
    try
    {

#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        // Check license
        std::auto_ptr< FlexLmLicense > license_runtime( FlexLmLicense::CheckLicense( "sword-runtime", 1.0f, "license.dat;.", FlexLmLicense::eCheckModeSilent ) );
        std::auto_ptr< FlexLmLicense > license_dispatch( FlexLmLicense::CheckLicense( "sword-dispatcher", 1.0f, "license.dat;.", FlexLmLicense::eCheckModeSilent ) );
        try
        {
            maxConnections = license_dispatch->GetAuthorisedUsers();
        }
        catch( FlexLmLicense::LicenseError& )
        {
            maxConnections = 1;
        }
#endif

        Application app( argc, argv, maxConnections );
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
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( LPSTR lpCmdLine ) 
{
    tools::WinArguments winArgs( lpCmdLine );
    return Run( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
}

//-----------------------------------------------------------------------------
// Name: Winmain
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

//-----------------------------------------------------------------------------
// Name: main
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int main( int argc, char** argv )
{
	return Run( argc, argv );
}
