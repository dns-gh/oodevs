// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************
#include "clients_gui_pch.h"
#include "GamingCommonConfig.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

void gui::AddGamingCommonOptions( po::options_description& desc,
        GamingCommonConfig& config )
{
    desc.add_options()
        ( "mapnik", po::value( &config.hasMapnik )->zero_tokens(),
             "enable mapnik layer" );
}
