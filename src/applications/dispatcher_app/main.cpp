// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include <tools/Exception.h>
#include "tools/WinArguments.h"
#include <windows.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace
{
bool HasOption( const std::vector< std::string >& args, const std::string& name )
{
    return std::find( args.begin(), args.end(), name ) != args.end();
}
}

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( LPSTR lpCmdLine )
{
    // Init logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int maxConnections = 10;
    int nResult = EXIT_FAILURE;
    try
    {
        // verbose mode
        std::vector< std::string > argv = boost::program_options:: split_winmain( lpCmdLine );
        
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        // Check license
        const bool verbose = HasOption( argv, "--verbose" );
        license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
        license_gui::LicenseDialog::CheckLicense( "sword-dispatcher", !verbose, &maxConnections );
#endif

        // Execute dispatcher
        tools::SetCodec();
        tools::WinArguments winArgs( lpCmdLine );
        Application app( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ), maxConnections );
        nResult = app.Execute( HasOption( argv, "--test" ) );
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
