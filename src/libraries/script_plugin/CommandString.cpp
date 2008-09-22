// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "CommandString.h"
#include <boost/regex.hpp>

using namespace script;

// -----------------------------------------------------------------------------
// Name: CommandString constructor
// Created: SBO 2008-06-30
// -----------------------------------------------------------------------------
CommandString::CommandString( const std::string& message )
{
    boost::regex rx( "^/(\\w+)|'([^']*)'|\"([^\"]*)\"|(\\w+)" );
    boost::sregex_iterator end;
    for( boost::sregex_iterator it( message.begin(), message.end(), rx ); it != end; ++it )
        for( unsigned int i = 1; i < it->size(); ++i )
            if( (*it)[i].matched )
            {
                const std::string match( (*it)[i].first, (*it)[i].second );
                args_.push_back( match );
            }
}

// -----------------------------------------------------------------------------
// Name: CommandString destructor
// Created: SBO 2008-06-30
// -----------------------------------------------------------------------------
CommandString::~CommandString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandString::Command
// Created: SBO 2008-06-30
// -----------------------------------------------------------------------------
std::string CommandString::Command() const
{
    return Arg( 0 );
}

// -----------------------------------------------------------------------------
// Name: CommandString::Arg
// Created: SBO 2008-06-30
// -----------------------------------------------------------------------------
std::string CommandString::Arg( unsigned int index ) const
{
    return index < args_.size() ? args_.at( index ) : "";
}
