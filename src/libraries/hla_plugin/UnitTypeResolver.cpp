// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnitTypeResolver.h"
#include "dispatcher/Logger_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/AgentType.h"

using namespace plugins::hla;

namespace
{
    unsigned long GetDefault( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::AgentType, std::string >& agentTypeResolver )
    {
        std::string defaultType;
        entityTypeResolver.Resolve( rpr::EntityType(), defaultType );
        const kernel::AgentType* agentType = agentTypeResolver.Find( defaultType );
        if( agentType == 0 )
            throw std::runtime_error( "Agent type identifier '" + defaultType + "' not found, please check your physical model." );
        return agentType->GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver constructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
UnitTypeResolver::UnitTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::AgentType, std::string >& agentTypeResolver,
                                dispatcher::Logger_ABC& logger )
    : entityTypeResolver_( entityTypeResolver )
    , agentTypeResolver_ ( agentTypeResolver )
    , defaultType_       ( GetDefault( entityTypeResolver, agentTypeResolver ) )
    , logger_            ( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver destructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
UnitTypeResolver::~UnitTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver::Resolve
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
unsigned long UnitTypeResolver::Resolve( const rpr::EntityType& type ) const
{
    std::string defaultType;
    if( !entityTypeResolver_.Resolve( type, defaultType ) )
        logger_.LogWarning( std::string( "Could not find Unit for EntityType ") + type.str() );
    const kernel::AgentType* agentType = agentTypeResolver_.Find( defaultType );
    if( agentType == 0 )
        return defaultType_;
    return agentType->GetId();
}
