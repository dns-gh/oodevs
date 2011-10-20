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
#include "bml_plugin/BmlPlugin.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Config.h"
#include <windows.h>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class SimulationPublisher_ABC;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: JMT 2011-09-19
// -----------------------------------------------------------------------------
BML_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& dynamicModel, const dispatcher::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& /*clients*/, const dispatcher::Config& /*config*/, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        dispatcher::Plugin_ABC* result = new plugins::bml::BmlPlugin( dynamicModel, staticModel, xis, simulation );
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
// Created: JMT 2011-09-19
// -----------------------------------------------------------------------------
BML_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
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
// Created: JMT 2011-09-19
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
