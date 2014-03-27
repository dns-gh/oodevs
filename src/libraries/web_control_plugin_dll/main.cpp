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
#include "dispatcher/Model_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Config.h"
#include <tools/Exception.h>
#include "web_control_plugin/WebPlugin.h"
#include <xeumeuleu/xml.hpp>
#include <windows.h>

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: BAX 2011-01-28
// -----------------------------------------------------------------------------
WEB_CONTROL_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& /*model*/, const dispatcher::StaticModel& /*staticModel*/,
                                                                   dispatcher::SimulationPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& /*clients*/,
                                                                   const dispatcher::Config& /*config*/, dispatcher::Logger_ABC& logger, xml::xistream& xis )
{
    try
    {
        logger.LogInfo( "Initialization..." );
        return new plugins::web_control::WebPlugin( publisher, xis );
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
// Created: BAX 2012-02-24
// -----------------------------------------------------------------------------
WEB_CONTROL_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin, dispatcher::Logger_ABC& logger )
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
// Created: BAX 2012-02-24
// -----------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE /*hModule*/, DWORD /*ulReasonForCall*/, LPVOID /*lpReserved*/ )
{
    return true;
}
