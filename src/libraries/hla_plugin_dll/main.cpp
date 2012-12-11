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
#include "hla_plugin/HlaPlugin.h"
#include <hla/HLAException.h>
#include "tools/Codec.h"
#include <tools/Exception.h>
#include <windows.h>

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class ClientPublisher_ABC;
    class StaticModel;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
HLA_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& dynamicModel, const dispatcher::StaticModel& staticModel,
                                                           dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients,
                                                           const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        dispatcher::Plugin_ABC* result = new plugins::hla::HlaPlugin( dynamicModel, staticModel, simulation, clients, config, xis, logger );
        logger.LogInfo( "Initialized!" );
        return result;
    }
    catch( const hla::HLAException& e )
    {
        logger.LogError( std::string( "Initialization failed cause hla exception: " ) + tools::GetExceptionMsg( e ) );
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
HLA_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
{
    try
    {
        logger.LogInfo( "Destruction..." );
        delete plugin;
        logger.LogInfo( "Destructed!" );
    }
    catch( const hla::HLAException& e )
    {
        logger.LogError( std::string( "Destruction failed cause hla exception: " ) + tools::GetExceptionMsg( e ) );
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
