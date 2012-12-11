// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DllConfig.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/Config.h"
#include "dispatcher/StaticModel.h"
#include "logistic_plugin/LogisticPlugin.h"
#include "logistic_plugin/NameResolver.h"
#include <tools/Exception.h>
#include <windows.h>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class StaticModel;
    class SimulationPublisher_ABC;
}

using plugins::logistic::NameResolver;

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: MMC 2012-08-20
// -----------------------------------------------------------------------------
LOGISTIC_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, const dispatcher::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& /* simulation */, dispatcher::ClientPublisher_ABC& /*clients*/, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        boost::shared_ptr<NameResolver> nameResolver( new NameResolver( model, staticModel ));
        dispatcher::Plugin_ABC* result = CreateLogisticPlugin( nameResolver, config, xis );
        logger.LogInfo( "Initialized!" );
        return result;
    }
    catch( const std::exception& e )
    {
        logger.LogError( std::string( "Initialization failed cause: " ) + tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        logger.LogError( "Initialization failed (unhandled error)." );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: MMC 2012-08-20
// -----------------------------------------------------------------------------
LOGISTIC_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
{
    try
    {
        logger.LogInfo( "Destruction..." );
        delete plugin;
        logger.LogInfo( "Destructed!" );
    }
    catch( const std::exception& e )
    {
        logger.LogError( std::string( "Destruction failed cause: " ) + tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        logger.LogError( "Destruction failed (unhandled error)." );
    }
}

// -----------------------------------------------------------------------------
// Name: DllMain
// Created: MMC 2012-08-20
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
