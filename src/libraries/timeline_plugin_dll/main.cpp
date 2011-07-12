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
#include "dispatcher/Config.h"
#include "dispatcher/StaticModel.h"
#include "timeline_plugin/TimelinePlugin.h"
#include <windows.h>

namespace dispatcher
{
    class StaticModel;
    class SimulationPublisher_ABC;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
TIMELINE_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, const dispatcher::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        dispatcher::Plugin_ABC* result = new plugins::timeline::TimelinePlugin( model, staticModel, simulation, config, xis, logger );
        logger.LogInfo( "Initialized!" );
        return result;
    }
    catch( std::exception& e )
    {
        logger.LogError( std::string( "Initialization failed cause: " ) + e.what() );
    }
    catch( ... )
    {
        logger.LogError( "Initialization failed (unhandled error)." );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
TIMELINE_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
{
    try
    {
        logger.LogInfo( "Destruction..." );
        delete plugin;
        logger.LogInfo( "Destructed!" );
    }
    catch( std::exception& e )
    {
        logger.LogError( std::string( "Destruction failed cause: " ) + e.what() );
    }
    catch( ... )
    {
        logger.LogError( "Destruction failed (unhandled error)." );
    }
}

// -----------------------------------------------------------------------------
// Name: DllMain
// Created: SBO 2011-01-27
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
