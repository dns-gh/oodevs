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
#include "dispatcher/Config.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include <windows.h>
#include <xeumeuleu/xml.hpp>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
DIS_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& /*simulation*/, const dispatcher::Config& config, xml::xistream& xis )
{
    MT_LOG_REGISTER_LOGGER( *new MT_FileLogger( config.BuildSessionChildFile( "dis_plugin.log" ).c_str(), MT_Logger_ABC::eLogLevel_All, true ) );
    return new plugins::dis::Plugin( model, config, xis );
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
DIS_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin )
{
    delete plugin;
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
