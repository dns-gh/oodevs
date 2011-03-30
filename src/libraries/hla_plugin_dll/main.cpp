// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DllConfig.h"
#include "hla_plugin/HlaPlugin.h"
#include <windows.h>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

// -----------------------------------------------------------------------------
// Name: CreateInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
HLA_PLUGIN_DLL_API dispatcher::Plugin_ABC* CreateInstance( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& /*simulation*/, const dispatcher::Config& config, xml::xistream& xis )
{
    return new plugins::hla::HlaPlugin( model, config, xis );
}

// -----------------------------------------------------------------------------
// Name: DestroyInstance
// Created: SBO 2011-01-28
// -----------------------------------------------------------------------------
HLA_PLUGIN_DLL_API void DestroyInstance( dispatcher::Plugin_ABC* plugin )
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
