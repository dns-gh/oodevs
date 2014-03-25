// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "LocalAgentResolver.h"
#include "dispatcher/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "dispatcher/Automat_ABC.h"


using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: LocalAgentResolver constructor
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
LocalAgentResolver::LocalAgentResolver()
{
    // NOTHINGs
}

// -----------------------------------------------------------------------------
// Name: LocalAgentResolver destructor
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
LocalAgentResolver::~LocalAgentResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalAgentResolver::Add
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
void LocalAgentResolver::Add( unsigned long simulationIdentifier, const std::string& objectIdentifier )
{
    identifiers_.insert( T_Identifiers::value_type( simulationIdentifier, objectIdentifier ) );
}

// -----------------------------------------------------------------------------
// Name: LocalAgentResolver::Resolve
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
std::string LocalAgentResolver::Resolve( unsigned long simulationIdentifier ) const
{
    T_Identifiers::left_const_iterator it = identifiers_.left.find( simulationIdentifier );
    if( it == identifiers_.left.end() )
        return "";
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: LocalAgentResolver::Resolve
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
unsigned long LocalAgentResolver::Resolve( const std::string& objectIdentifier ) const
{
    T_Identifiers::right_const_iterator it = identifiers_.right.find( objectIdentifier );
    if( it == identifiers_.right.end() )
        return 0;
    return it->second;
}

void LocalAgentResolver::Add( unsigned long simulationIdentifier, dispatcher::Agent_ABC& agent )
{
    agents_[ simulationIdentifier ] = &agent;
}

unsigned long LocalAgentResolver::ParentAutomat( unsigned long simulationIdentifier ) const
{
    T_Agents::const_iterator it = agents_.find( simulationIdentifier );
    if( agents_.end() == it )
        return 0;
    return it->second->GetSuperior().GetId();
}

unsigned long LocalAgentResolver::AgentType( unsigned long simulationIdentifier ) const
{
    T_Agents::const_iterator it = agents_.find( simulationIdentifier );
    if( agents_.end() == it )
        return 0;
    return it->second->GetType().GetId();
}
