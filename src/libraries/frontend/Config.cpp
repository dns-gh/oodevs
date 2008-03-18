// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Config.h"

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Config::Config()
{
    po::options_description desc( "Frontend options" );
    desc.add_options()
        ( "install" , po::value( &package_ ), "specify the package to install" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::GetPackageFile
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
std::string Config::GetPackageFile() const
{
    return package_;
}
