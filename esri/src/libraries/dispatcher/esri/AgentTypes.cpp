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
#include "AutomatType.h"
#include "SymbolFactory.h"
#include "PathTools.h"
#include "xeumeuleu/xml.h"
#include "ESRI_Config.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void AgentTypes::Load( const dispatcher::esri::Config& config )
{
    Purge();

    xml::xifstream xisSymbols( path_tools::BuildWorkingDirectoryPath( "symbols.xml" ) ); // $$$$ NLD 2007-01-12: 
    symbolFactory_ = new SymbolFactory( xisSymbols );

    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> start( "physical" );

    std::string agents, automats;
    xis >> content( "Pions", agents )
        >> content( "Automates", automats );

    ReadAgents( config.BuildPhysicalChildFile( agents ) );
    ReadAutomats( config.BuildPhysicalChildFile( automats ) );    
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void AgentTypes::Purge()
{
    Resolver< AgentType, std::string >::Clear();
    Resolver< AgentType >::DeleteAll();        
    Resolver< AutomatType, std::string >::Clear();
    Resolver< AutomatType >::DeleteAll();    
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
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( const std::string& agents )
{
    xifstream xis( agents );
    xis >> start( "Pions" )
            >> list( "Unite", *this, &AgentTypes::ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    AgentType* type = new AgentType( xis, *symbolFactory_ );
    Resolver< AgentType >               ::Register( type->GetId(), *type );
    Resolver< AgentType, std::string >  ::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomats
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomats( const std::string& automats )
{
    xifstream xis( automats );
    xis >> start( "Automates" )
            >> list( "Unite", *this, &AgentTypes::ReadAutomatType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomatType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomatType( xml::xistream& xis )
{
    AutomatType* type = new AutomatType( xis, *this );
    Resolver< AutomatType >                 ::Register( type->GetId(), *type ); 
    Resolver< AutomatType, std::string >    ::Register( type->GetName(), *type );
}