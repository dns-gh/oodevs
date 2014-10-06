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
CallsignResolver::CallsignResolver( xml::xisubstream xis )
    : addIdentifier_( xis.attribute< bool >( "marking-with-identifier", true ) )
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
void CallsignResolver::Add( unsigned long simulationIdentifier, const std::string& callsign, const std::vector< char >& uniqueId )
{
    identifiers_[ simulationIdentifier ] = std::make_pair( callsign, uniqueId );
    simulationIdentifiers_[ uniqueId ] = simulationIdentifier;
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::ResolveCallsign
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
std::string CallsignResolver::ResolveCallsign( unsigned long simulationIdentifier ) const
{
    T_Identifiers::const_iterator identifier = identifiers_.find( simulationIdentifier );
    if( identifier == identifiers_.end() )
        throw MASA_EXCEPTION( "Unknown simulation identifier '" + boost::lexical_cast< std::string >( simulationIdentifier ) + "'" );
    return identifier->second.first;
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::ResolveUniqueId
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
std::vector< char > CallsignResolver::ResolveUniqueId( unsigned long simulationIdentifier ) const
{
    T_Identifiers::const_iterator identifier = identifiers_.find( simulationIdentifier );
    if( identifier == identifiers_.end() )
        throw MASA_EXCEPTION( "Unknown simulation identifier '" + boost::lexical_cast< std::string >( simulationIdentifier ) + "'" );
    return identifier->second.second;
}

// -----------------------------------------------------------------------------
// Name: CallsignResolver::ResolveSimulationIdentifier
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
unsigned long CallsignResolver::ResolveSimulationIdentifier( const std::vector< char >& uniqueId ) const
{
    T_SimulationIdentifiers::const_iterator identifier = simulationIdentifiers_.find( uniqueId );
    if( identifier == simulationIdentifiers_.end() )
        throw MASA_EXCEPTION( "Unknown unique identifier '" + std::string( uniqueId.begin(), uniqueId.end() ) + "'" );
    return identifier->second;
}

std::string CallsignResolver::Generate( const std::string& name, unsigned long simId ) const
{
    if( addIdentifier_ )
        return name + boost::lexical_cast< std::string >( simId );
    return name;
}
