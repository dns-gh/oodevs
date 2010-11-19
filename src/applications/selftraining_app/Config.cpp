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

#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
Config::Config()
    : profile_( eAdministrator )
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

// -----------------------------------------------------------------------------
// Name: Config::SetProfile
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void Config::SetProfile( EProfile profile )
{
    profile_ = profile;
}

// -----------------------------------------------------------------------------
// Name: Config::GetProfile
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
Config::EProfile Config::GetProfile() const
{
    return profile_;
}
