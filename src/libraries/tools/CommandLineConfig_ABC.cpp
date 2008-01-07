// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "CommandLineConfig_ABC.h"

#include <iostream>
#include <sstream>

#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

using namespace tools;

// -----------------------------------------------------------------------------
// Name: CommandLineConfig_ABC constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
CommandLineConfig_ABC::CommandLineConfig_ABC()
    : options_( new po::options_description( "Default options" ) )
    , values_ ( new po::variables_map() )
{
    options_->add_options()( "help,h", "display help" );
}

// -----------------------------------------------------------------------------
// Name: CommandLineConfig_ABC destructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
CommandLineConfig_ABC::~CommandLineConfig_ABC()
{
    delete options_;
    delete values_;
}

// -----------------------------------------------------------------------------
// Name: CommandLineConfig_ABC::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void CommandLineConfig_ABC::Parse( int argc, char** argv )
{
    po::store( po::command_line_parser( argc, argv ).options( *options_ ).run(), *values_ );
    po::notify( *values_ );

    if( values_->count( "help" ) )
    {
        std::stringstream ss;
        ss << *options_;
        throw std::runtime_error( ss.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: CommandLineConfig_ABC::AddOptions
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void CommandLineConfig_ABC::AddOptions( boost::program_options::options_description& options )
{
    options_->add( options );
}

// -----------------------------------------------------------------------------
// Name: CommandLineConfig_ABC::IsSet
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
bool CommandLineConfig_ABC::IsSet( const std::string& option ) const
{
    return values_ && values_->count( option );
}
