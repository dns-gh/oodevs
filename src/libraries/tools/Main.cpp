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
#include <windows.h>

namespace
{
    const char* GetFileName( MT_Logger_ABC::E_Type type )
    {
        switch( type )
        {
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
}

std::shared_ptr< MT_Logger_ABC > tools::Initialize( const tools::WinArguments& winArgs, MT_Logger_ABC::E_Type type )
{
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    const std::shared_ptr< MT_FileLogger > logger(
        new MT_FileLogger( debugDir / GetFileName( type ) + ".log",
            1, 0, MT_Logger_ABC::eLogLevel_All,
            false, type ),
        []( MT_FileLogger* logger )
        {
            MT_LOG_UNREGISTER_LOGGER( *logger );
            delete logger;
        } );
    MT_LOG_REGISTER_LOGGER( *logger );
    MT_CrashHandler::SetRootDirectory( debugDir );
    return logger;
}
