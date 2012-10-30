// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Config.h"
#include "license_gui/LicenseDialog.h"

namespace
{
// -----------------------------------------------------------------------------
// Name: CheckLicenseFeature
// Created: BAX 2012-10-22
// -----------------------------------------------------------------------------
bool CheckLicenseFeature( const std::string& feature )
{
    try
    {
#if !defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( feature, true, 0, 0 );
#endif
        return true;
    }
    catch( std::exception& /*e*/ )
    {
        return false;
    }
}
}

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
Config::Config()
    : hasAuthoring_( CheckLicenseFeature( "sword-authoring" ) )
    , hasTerrainGeneration_( CheckLicenseFeature( "sword-terrain-generation" ) )
    , hasPreparation_( CheckLicenseFeature( "sword-preparation" ) )
    , hasRuntime_( CheckLicenseFeature( "sword-runtime" ) )
    , hasReplayer_( CheckLicenseFeature( "sword-replayer" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}