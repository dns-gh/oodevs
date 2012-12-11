// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DllConfig.h"
#include "dis_plugin/Plugin.h"
#include "dispatcher/Logger_ABC.h"
#include "tools/Codec.h"
#include <tools/Exception.h>
#include <windows.h>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class SimulationPublisher_ABC;
    class StaticModel;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
DIS_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, const dispatcher::StaticModel& /*staticModel*/, dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        return new plugins::dis::Plugin( model, config, logger, xis );
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
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
DIS_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
{
    try
    {
        logger.LogInfo( "Destruction..." );
        delete plugin;
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
// Created: SBO 2011-01-27
// -----------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE /*hModule*/, DWORD ulReasonForCall, LPVOID /*lpReserved*/ )
{
    tools::SetCodec();
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
