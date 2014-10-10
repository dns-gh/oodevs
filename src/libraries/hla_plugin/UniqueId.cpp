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
#include "LocalAgentResolver_ABC.h"
#include "CallsignResolver_ABC.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4724 )
#endif

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UniqueIdSerializer constructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
UniqueIdSerializer::UniqueIdSerializer( int netnVersion )
    : netnVersion_( netnVersion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniqueIdSerializer destructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
UniqueIdSerializer::~UniqueIdSerializer()
{
    // NOTHING
}

std::string UniqueIdSerializer::GetAgentId( const NETN_UUID& uniqueID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
{
    try
    {
        unsigned long simId = callsignResolver.ResolveSimulationIdentifier( uniqueID.data() );
        return agentResolver.Resolve( simId );
    }
    catch( const std::exception& )
    {
        // NOTHING
    }
    return std::string("");
}

std::vector< char > UniqueIdSerializer::GenerateUniqueId( std::size_t sz )
{
    static boost::uuids::random_generator gen;
    boost::uuids::uuid uid = gen();
    std::vector< char > retval( sz, 0);
    std::copy( uid.begin(), uid.begin() + sz, retval.begin() );
    return retval;
}
