// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Main.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/WinArguments.h"
#include "tools/Exception.h"
#include <tools/Path.h>
#include <tools/win32/CrashHandler.h>
#include <windows.h>

namespace
{
    void CrashHandler( EXCEPTION_POINTERS* exception )
    {
        MT_CrashHandler::ExecuteHandler( exception );
    }

    const char* GetFileName( MT_Logger_ABC::E_Type type )
    {
        switch( type )
        {
        case MT_Logger_ABC::eAdaptation : return "adaptation";
        case MT_Logger_ABC::ePreparation : return "preparation";
        case MT_Logger_ABC::eFrontend : return "selftraining";
        case MT_Logger_ABC::eDispatcher : return "dispatcher";
        case MT_Logger_ABC::eGaming : return "gaming";
        case MT_Logger_ABC::eReplayer : return "replayer";
        case MT_Logger_ABC::eSimulation : return "simulation";
        default:
            throw MASA_EXCEPTION( "unsupported logger type" );
        }
    }

    MT_Logger_ABC::E_LogLevel GetLogLevel( const std::string& level )
    {
        if( level == "fatal" )
            return MT_Logger_ABC::eLogLevel_FatalError;
        if( level == "error" )
            return MT_Logger_ABC::eLogLevel_Error;
        if( level == "warning" )
            return MT_Logger_ABC::eLogLevel_Warning;
        if( level == "message" )
            return MT_Logger_ABC::eLogLevel_Message;
        if( level == "info" )
            return MT_Logger_ABC::eLogLevel_Info;
        if( level == "verbose" )
            return MT_Logger_ABC::eLogLevel_Verbose;
        if( level == "debug" )
            return MT_Logger_ABC::eLogLevel_Debug;
        return MT_Logger_ABC::eLogLevel_None;
    }

    std::shared_ptr< MT_Logger_ABC > CreateLogger(
        MT_Logger_ABC::E_Type type,
        MT_Logger_ABC::E_LogLevel level,
        const tools::Path& debugDir )
    {
        if( level != MT_Logger_ABC::eLogLevel_None )
            try
            {
                debugDir.CreateDirectories();
                std::shared_ptr< MT_Logger_ABC > logger(
                    new MT_FileLogger(
                        debugDir / GetFileName( type ) + ".log",
                        1, 0, level, false, type ),
                    []( MT_FileLogger* logger )
                    {
                        MT_LOG_UNREGISTER_LOGGER( *logger );
                        delete logger;
                    } );
                MT_LOG_REGISTER_LOGGER( *logger );
                return logger;
            }
            catch( ... )
            {}
        return std::shared_ptr< MT_Logger_ABC >();
    }

    template< typename M >
    int CallMain( const M& main, const tools::WinArguments& winArgs )
    {
        __try
        {
            return main( winArgs );
        }
        __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
        {
            return EXIT_FAILURE;
        }
    }
}

int tools::Main(
    const tools::WinArguments& winArgs,
    MT_Logger_ABC::E_Type type,
    bool silentCrash,
    int(*main)( const tools::WinArguments& ) )
{
    if( silentCrash )
    {
        tools::InitCrashHandler( &CrashHandler );
        tools::InitPureCallHandler();
    }
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    MT_CrashHandler::SetRootDirectory( debugDir );
    const MT_Logger_ABC::E_LogLevel level = GetLogLevel( winArgs.GetOption( "--log-level", "fatal" ) );
    const auto logger = CreateLogger( type, level, debugDir );
    if( silentCrash )
        return main( winArgs );
    return CallMain( main, winArgs );
}
