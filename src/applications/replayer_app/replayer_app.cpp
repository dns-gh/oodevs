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
typedef std::vector< std::string > T_Args;
std::string GetOption( const T_Args& args, const std::string& name, const std::string& def )
{
    T_Args::const_iterator it = std::find( args.begin(), args.end(), name );
    if( it != args.end() )
        if( ++it != args.end() )
            return *it;
    return def;
}

bool HasFlag( const T_Args& args, const std::string& name )
{
    return std::find( args.begin(), args.end(), name ) != args.end();
}
}

int RunReplayer(  HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    const T_Args argv = boost::program_options::split_winmain( lpCmdLine );
    const std::string debugRoot = GetOption( argv, "--debug-dir", "./Debug" );

    MT_CrashHandler::SetRootDirectory( debugRoot );

    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    try
    {
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( "sword-replayer", !HasFlag( argv, "verbose" ) );
#endif
        tools::SetCodec();
        App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow, !HasFlag( argv, "--no-log" ) );
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
