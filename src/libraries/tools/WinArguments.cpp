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
#include <tools/Helpers.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

using namespace tools;

// -----------------------------------------------------------------------------
// Name: WinArguments constructor
// Created: RDS 2008-07-22
// -----------------------------------------------------------------------------
WinArguments::WinArguments( const std::wstring& arguments )
{
    std::vector< std::wstring > wargv = boost::program_options::split_winmain( arguments );
    argv_.push_back( "dummy-application.exe" ); // $$$$ ABR 2013-03-13: here to simulate a classic command line for boost parser
    for( auto it = wargv.begin(); it != wargv.end(); ++it )
        argv_.push_back( tools::FromUnicodeToUtf8( *it ) );
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

// -----------------------------------------------------------------------------
// Name: WinArguments::HasOption
// Created: ABR 2013-03-12
// -----------------------------------------------------------------------------
bool WinArguments::HasOption( const std::string& name ) const
{
    return std::find( argv_.begin(), argv_.end(), name ) != argv_.end();
}

// -----------------------------------------------------------------------------
// Name: WinArguments::GetOption
// Created: ABR 2013-03-12
// -----------------------------------------------------------------------------
std::string WinArguments::GetOption( const std::string& name, const std::string& defaultValue /* = "" */ ) const
{
    for( auto arg = argv_.begin() + 1; arg != argv_.end(); ++arg )
    {
        if( arg->find(name) != 0 )
            continue;
        if( arg->size() == name.size() )
        {
            // "--foo bar" form
            if( (arg + 1) != argv_.end() )
                return *(arg + 1);
        }
        else
        {
            // "--foo=bar" form
            if( (*arg)[name.size()] == '=' )
                return arg->substr(name.size() + 1 );
        }
    }
    return defaultValue;
}

std::string WinArguments::GetCommandLine() const
{
    std::stringstream s;
    std::copy( argv_.begin() + 1, argv_.end(),
        std::ostream_iterator< std::string >( s, " " ) );
    return s.str();
}
