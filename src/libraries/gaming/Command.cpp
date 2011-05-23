// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Command.h"
#include <boost/regex.hpp>

// -----------------------------------------------------------------------------
// Name: Command constructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
Command::Command( const std::string& from, const std::string& to, const std::string& text )
    : source_( from )
    , target_( to )
{
    Parse( text );
    if( arguments_.empty() )
        throw std::runtime_error( __FUNCTION__ ": Invalid command: " + text );
}

// -----------------------------------------------------------------------------
// Name: Command destructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
Command::~Command()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Command::Parse
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void Command::Parse( const std::string& text )
{
    boost::regex rx( "^/(\\w+)|'([^']*)'|\"([^\"]*)\"|([^\\s'\"]+)" );
    boost::sregex_iterator end;
    for( boost::sregex_iterator it( text.begin(), text.end(), rx ); it != end; ++it )
        for( unsigned int i = 1; i < it->size(); ++i )
            if( (*it)[i].matched )
            {
                const std::string match( (*it)[i].first, (*it)[i].second );
                arguments_.push_back( match );
            }
}

// -----------------------------------------------------------------------------
// Name: Command::Source
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
std::string Command::Source() const
{
    return source_;
}

// -----------------------------------------------------------------------------
// Name: Command::Target
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
std::string Command::Target() const
{
    return target_;
}

// -----------------------------------------------------------------------------
// Name: Command::Name
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
std::string Command::Name() const
{
    return arguments_.empty() ? "" : arguments_.front();
}

// -----------------------------------------------------------------------------
// Name: Command::Argument
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
std::string Command::Argument( unsigned int index ) const
{
    return arguments_.size() > index ? arguments_.at( index ) : "";
}

// -----------------------------------------------------------------------------
// Name: Command::ArgumentCount
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
unsigned int Command::ArgumentCount() const
{
    return static_cast< unsigned >( arguments_.empty() ? 0 : arguments_.size() - 1 );
}
