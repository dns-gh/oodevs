// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DllConfig.h"
#include "dispatcher/Config.h"
#include "vrforces_plugin/Plugin.h"
#include <fstream>
#include <windows.h>

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
VRFORCES_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& simulation, const dispatcher::Config& config, xml::xistream& xis )
{
    const std::string logFile( config.BuildSessionChildFile( "vrforces_dll_crash.log" ) );
    try
    {
        return new plugins::vrforces::Plugin( model, simulation, config, xis );
    }
    catch( std::exception& e )
    {
        std::ofstream log( logFile.c_str() );
        log << "Initialization failed cause: " << e.what() << std::endl;
    }
    catch( ... )
    {
        std::ofstream log( logFile.c_str() );
        log << "Initialization failed (unhandled error)." << std::endl;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
VRFORCES_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin )
{
    try
    {
        delete plugin;
    }
    catch( ... ) {}
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
