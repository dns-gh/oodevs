// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DllConfig.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Config.h"
#include "PluginProcessHandler.h"
#include <windows.h>

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
    class StaticModel;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: JCR 2011-09-19
// -----------------------------------------------------------------------------
EDXL_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& /*dynamicModel*/, const dispatcher::StaticModel& /*staticModel*/, dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( LOG_MESSAGE( "Registrating..." ) );
        plugins::edxl::PluginProcessHandler* result = new plugins::edxl::PluginProcessHandler( config, "edxlhave_app.exe", logger, xis );
        logger.LogInfo( LOG_MESSAGE( "Registred." ) );
        return result;
    }
    catch( std::exception& e )
    {
        logger.LogError( LOG_MESSAGE( "Initialization failed cause: " + e.what() ) );
    }
    catch( ... )
    {
        logger.LogError( LOG_MESSAGE( "Initialization failed (unhandled error)." ) );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: JCR 2011-09-19
// -----------------------------------------------------------------------------
EDXL_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
{
    try
    {
        logger.LogInfo( LOG_MESSAGE( "Unloading..." ) );
        delete plugin;
        logger.LogInfo( LOG_MESSAGE( "Unloaded." ) );
    }
    catch( std::exception& e )
    {
        logger.LogError( LOG_MESSAGE( "Destruction failed cause: " ) + e.what() );
    }
    catch( ... )
    {
        logger.LogError( LOG_MESSAGE( "Destruction failed (unhandled error)." ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DllMain
// Created: JCR 2011-09-19
// -----------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE /*hModule*/, DWORD ulReasonForCall, LPVOID /*lpReserved*/ )
{
    switch( ulReasonForCall )
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
