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
#pragma warning( disable: 4127 4511 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
Config::Config()
    : listServerPort_( 31000 )
    , listClientPort_( 31001 )
{
    po::options_description desc( "Self training options" );
    desc.add_options()
        ( "listserver" , po::value( &listServerPort_ )->default_value( 31000 ), "specify the port number listing multiplayer scenarios" )
        ( "listclient" , po::value( &listClientPort_ )->default_value( 31001 ), "specify the port number to query multiplayer scenarios list" )
    ;
    AddOptions( desc );
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
// Name: Config::GetListServerPort
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
unsigned short Config::GetListServerPort() const
{
    return listServerPort_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetListClientPort
// Created: LDC 2008-10-27
// -----------------------------------------------------------------------------
unsigned short Config::GetListClientPort() const
{
    return listClientPort_;
}
