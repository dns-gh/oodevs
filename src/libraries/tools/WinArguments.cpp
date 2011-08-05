// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "WinArguments.h"

#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

using namespace tools;

// -----------------------------------------------------------------------------
// Name: WinArguments constructor
// Created: RDS 2008-07-22
// -----------------------------------------------------------------------------
WinArguments::WinArguments( const std::string& arguments )
{
    argv_ = boost::program_options::split_winmain( arguments );
    argv_.insert( argv_.begin(), "simulation_app.exe" );
    std::transform( argv_.begin(), argv_.end(), std::back_inserter( cArgv_ ), std::mem_fun_ref( &std::string::c_str ) );
}

// -----------------------------------------------------------------------------
// Name: WinArguments destructor
// Created: RDS 2008-07-22
// -----------------------------------------------------------------------------
WinArguments::~WinArguments()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WinArguments::Argv
// Created: RDS 2008-07-22
// -----------------------------------------------------------------------------
const char* const* WinArguments::Argv() const
{
    return cArgv_.empty() ? 0 : &cArgv_.front();
}

// -----------------------------------------------------------------------------
// Name: WinArguments::Argc
// Created: RDS 2008-07-22
// -----------------------------------------------------------------------------
int WinArguments::Argc() const
{
    return (int)cArgv_.size();
}
