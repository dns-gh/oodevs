// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig constructor
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
ADN_GeneralConfig::ADN_GeneralConfig( const tools::Path& defaultRoot )
    : tools::GeneralConfig( defaultRoot )
{
    po::options_description desc( "Dev options" );
    desc.add_options()
        ( "dev", "activate dev mode" );
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig destructor
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
ADN_GeneralConfig::~ADN_GeneralConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::Parse
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
void ADN_GeneralConfig::Parse( int argc, char** argv )
{
    tools::GeneralConfig::Parse( argc, argv );
    devMode_ = IsSet( "dev" );
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::IsDevMode
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
bool ADN_GeneralConfig::IsDevMode() const
{
    return devMode_;
}
