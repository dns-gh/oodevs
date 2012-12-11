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
#include <boost/foreach.hpp>

namespace
{
bool CheckSingleFeature( const std::string& feature, bool silent )
{
    bool rpy = true;
#if !defined( NO_LICENSE_CHECK )
    QSet< QWidget* > set;
    try
    {
        if( !silent )
            BOOST_FOREACH( QWidget* w, QApplication::topLevelWidgets() )
            {
                if( !w->isVisible() )
                    continue;
                set.insert( w );
                w->hide();
            }
        license_gui::LicenseDialog::CheckLicense( feature, silent, 0, 0 );
    }
    catch( const std::exception& /*e*/ )
    {
        rpy = false;
    }
    BOOST_FOREACH( QWidget* w, set )
        w->show();
#endif
    return rpy;
}
}

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
Config::Config()
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

namespace
{
    std::string GetFeature( Feature feature )
    {
        switch( feature )
        {
            case FEATURE_AUTHORING:          return "sword-authoring";
            case FEATURE_TERRAIN_GENERATION: return "sword-terrain-generation";
            case FEATURE_PREPARATION:        return "sword-preparation";
            case FEATURE_RUNTIME:            return "sword-runtime";
            case FEATURE_REPLAYER:           return "sword-replayer";
        }
        return "unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Config::CheckFeature
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
bool Config::CheckFeature( Feature feature ) const
{
    return CheckSingleFeature( GetFeature( feature ), false );
}

// -----------------------------------------------------------------------------
// Name: Config::HasFeature
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
bool Config::HasFeature( Feature feature ) const
{
    return CheckSingleFeature( GetFeature( feature ), true );
}