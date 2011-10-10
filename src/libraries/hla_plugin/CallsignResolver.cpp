// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "CallsignResolver.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: CallsignResolver constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
CallsignResolver::CallsignResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
CallsignResolver::~CallsignResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::Add
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void CallsignResolver::Add( unsigned long simulationIdentifier, const std::string& callsign, const std::string& uniqueId )
{
    identifiers_[ simulationIdentifier ] = std::make_pair( callsign, uniqueId );
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::ResolveCallsign
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
std::string CallsignResolver::ResolveCallsign( unsigned long simulationIdentifier ) const
{
    T_Identifiers::const_iterator identifier = identifiers_.find( simulationIdentifier );
    if( identifier == identifiers_.end() )
        throw std::runtime_error( "Unknown simulation identifier '" + boost::lexical_cast< std::string >( simulationIdentifier ) + "'" );
    return identifier->second.first;
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::ResolveUniqueId
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
std::string CallsignResolver::ResolveUniqueId( unsigned long simulationIdentifier ) const
{
    T_Identifiers::const_iterator identifier = identifiers_.find( simulationIdentifier );
    if( identifier == identifiers_.end() )
        throw std::runtime_error( "Unknown simulation identifier '" + boost::lexical_cast< std::string >( simulationIdentifier ) + "'" );
    return identifier->second.second;
}
