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
#include <tools/StackContext.h>
#include <windows.h>

namespace
{
    tools::StackContext context;

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

    struct LogLevel
    {
        std::string name;
        int value;
    };

    // Sorted by decreasing importance level
    const LogLevel levels[] =
    {
        { "fatal", MT_Logger_ABC::eLogLevel_FatalError },
        { "error", MT_Logger_ABC::eLogLevel_Error },
        { "warning", MT_Logger_ABC::eLogLevel_Warning },
        { "message", MT_Logger_ABC::eLogLevel_Message },
        { "info", MT_Logger_ABC::eLogLevel_Info },
        { "verbose", MT_Logger_ABC::eLogLevel_Verbose },
        { "debug", MT_Logger_ABC::eLogLevel_Debug },
    };

    std::shared_ptr< MT_Logger_ABC > CreateLogger(
        MT_Logger_ABC::E_Type type,
        int level,
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

int tools::GetLogLevel( const std::string& name )
{
    int level = 0;
    for( size_t i = 0; i != COUNT_OF( levels ); ++i )
    {
        level |= levels[i].value;
        if( levels[i].name == name )
            return level;
    }
    return 0;
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
    const auto level = GetLogLevel( winArgs.GetOption( "--log-level", "fatal" ) );
    const auto logger = CreateLogger( type, level, debugDir );
    if( silentCrash )
        return main( winArgs );
    return CallMain( main, winArgs );
}
