// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Config.h"

#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config()
    : tools::GeneralConfig()
{
    po::options_description desc( "Dispatcher options" );
//    desc.add_options()
//        ( "nothing"     , "specify checkpoint to load"               )
//        ( "test"        ,                                                  "test mode: loading + first tick"          )
//        ( "testdata"    ,                                                  "test mode: load models only (no terrain)" )
//    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}
