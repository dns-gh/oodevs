// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Workspace.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Workspace constructor
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
crossbow::Workspace::Workspace()
{
    // #if _ARCGIS_VERSION_ >= 9.2  // $$$$ JCR 2007-06-14: do not use license checking on arcgis version lower than 9.2
    ::CoInitialize( NULL );
    if ( ! InitializeLicence( esriLicenseProductCodeEngine ) )
        if ( ! InitializeLicence( esriLicenseProductCodeArcView ) )
            if ( ! InitializeLicence( esriLicenseProductCodeArcEditor ) )
                if ( ! InitializeLicence( esriLicenseProductCodeArcInfo ) )
                    throw std::runtime_error( "Unable to initialize license manager" );
}

// -----------------------------------------------------------------------------
// Name: Workspace destructor
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
crossbow::Workspace::~Workspace()
{
    IAoInitializePtr license( CLSID_AoInitialize );
    license->Shutdown();
    license.Release();
    ::CoUninitialize();
}

// -----------------------------------------------------------------------------
// Name: Workspace::InitializeLicence
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
bool crossbow::Workspace::InitializeLicence( esriLicenseProductCode product )
{
    IAoInitializePtr license( CLSID_AoInitialize );
    esriLicenseStatus status = esriLicenseFailure;
    license->IsProductCodeAvailable( product, &status );
    if ( status == esriLicenseAvailable )
        license->Initialize( product, &status );
    return ( status == esriLicenseCheckedOut );
}
