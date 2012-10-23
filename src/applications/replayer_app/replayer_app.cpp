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
#include "tools/WinArguments.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <windows.h>
#include <boost/optional.hpp>

namespace
{
typedef std::vector< std::string > T_Args;
boost::optional< std::string > GetOption( const T_Args& args, const std::string& name )
{
    T_Args::const_iterator it = std::find( args.begin(), args.end(), name );
    if( it != args.end() )
        if( ++it != args.end() )
            return *it;
    return boost::none;
}
}

int RunReplayer(  HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    const T_Args argv = boost::program_options:: split_winmain( lpCmdLine );
    boost::optional< std::string > opt = GetOption( argv, "--debug-dir" );
    const std::string debugRoot = opt == boost::none ? "./Debug" : *opt;
    const bool replayLog = std::find( argv.begin(), argv.end(), "--no-log" ) == argv.end();

    MT_CrashHandler::SetRootDirectory( debugRoot );

    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    bool verbose = false;
    try
    {
        // verbose mode
        verbose = std::find( argv.begin(), argv.end(), "--verbose" ) != argv.end();

#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( "sword-replayer", !verbose );
#endif
        tools::SetCodec();
        App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow, replayLog );
        app.Execute();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
        nResult = EXIT_FAILURE;
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    __try
    {
        return RunReplayer( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
    return 0;
}
