// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UniqueId.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UniqueId constructor
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
UniqueId::UniqueId()
{
    ::memset( identifier_, 0, sizeof( identifier_ ) );
}

// -----------------------------------------------------------------------------
// Name: UniqueId constructor
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
UniqueId::UniqueId( const std::string& identifier )
{
    ::memset( identifier_, 0, sizeof( identifier_ ) );
    const std::size_t length = std::min( sizeof( identifier_ ), identifier.length() );
    ::memcpy( identifier_, identifier.c_str(), length );
}

// -----------------------------------------------------------------------------
// Name: UniqueId destructor
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
UniqueId::~UniqueId()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniqueId::str
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
std::string UniqueId::str() const
{
    std::string result;
    for( unsigned int i = 0; i < 11; ++i )
    {
        if( !identifier_[ i ] )
            return result;
        result.push_back( identifier_[ i ] );
    }
    return result;
}
