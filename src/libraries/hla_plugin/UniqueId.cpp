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
