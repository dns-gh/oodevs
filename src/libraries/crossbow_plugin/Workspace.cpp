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
    InitializeLicence( esriLicenseProductCodeEngine );
}

// -----------------------------------------------------------------------------
// Name: Workspace destructor
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
crossbow::Workspace::~Workspace()
{
    if( license_ )
        license_->Shutdown();
    ::CoUninitialize();
}

// -----------------------------------------------------------------------------
// Name: Workspace::InitializeLicence
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void crossbow::Workspace::InitializeLicence( esriLicenseProductCode product )
{
    if( FAILED( license_.CreateInstance( CLSID_AoInitialize ) ) )
        throw std::runtime_error( "Unable to initialize license manager" );
    esriLicenseStatus status;
    if( FAILED( license_->IsProductCodeAvailable( product, &status ) ) )
        throw std::runtime_error( "No product available" );
    if( FAILED( license_->Initialize( product, &status ) ) )
        throw std::runtime_error( "Unable to retrieve license information" );
}
