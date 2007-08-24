// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentTypes.h"
#include "AgentType.h"
#include "SymbolFactory.h"
#include "Config.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes( const dispatcher::Config& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void AgentTypes::Load( const dispatcher::Config& config )
{
    Purge();

    xml::xifstream xisSymbols( dispatcher::Config::BuildWorkingDirectoryChildFile( "symbols.xml" ) ); // $$$$ NLD 2007-01-12:
    symbolFactory_ = new SymbolFactory( xisSymbols );

    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> start( "physical" );

    std::string agents;
    xis >> start( "units" ) >> attribute( "file", agents );

    ReadAgents( config.BuildPhysicalChildFile( agents ) );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::~AgentTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Purge
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
void AgentTypes::Purge()
{
    for( CIT_AgentTypes it = agentTypes_.begin(); it != agentTypes_.end(); ++it )
        delete it->second;
    agentTypes_.clear();
}

 // -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( const std::string& agents )
{
    xifstream xis( agents );
    xis >> start( "units" )
            >> list( "unit", *this, &AgentTypes::ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    AgentType* type = new AgentType( xis, *symbolFactory_ );
    agentTypes_[type->GetId()] = type;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Get
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
const AgentType& AgentTypes::Get( unsigned long id ) const
{
    CIT_AgentTypes it = agentTypes_.find( id );
    if( it != agentTypes_.end() )
        return *it->second;
    throw std::runtime_error( "Agent type does not exist" );
}
