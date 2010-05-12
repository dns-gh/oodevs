// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentKnowledgeConverter.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Automat.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "PopulationKnowledge.h"
#include "tools/Resolver.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::AgentKnowledgeConverter( const dispatcher::Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::~AgentKnowledgeConverter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner )
{
    const kernel::Entity_ABC& group = FindGroup( owner );
    tools::Iterator< const dispatcher::AgentKnowledge& > it = model_.agentKnowledges_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::AgentKnowledge& k = it.NextElement();
        if( & k.knowledgeGroup_ == &group && &k.agent_ == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner )
{
    const kernel::Entity_ABC& group = FindGroup( owner );
    tools::Iterator< const dispatcher::PopulationKnowledge& > it = model_.populationKnowledges_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::PopulationKnowledge& k = it.NextElement();
        if( & k.knowledgeGroup_ == &group && &k.population_ == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindAgent
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::FindAgent( unsigned long , const kernel::Entity_ABC& )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindPopulation
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::FindPopulation( unsigned long , const kernel::Entity_ABC& )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::AgentKnowledge_ABC& , const kernel::Entity_ABC& )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::PopulationKnowledge_ABC& , const kernel::Entity_ABC& )
{
    return 0;
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindGroup
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& AgentKnowledgeConverter::FindGroup( const kernel::Entity_ABC& owner )
{
    if( const dispatcher::Automat* automat = dynamic_cast< const dispatcher::Automat* >( &owner ) )
        return automat->GetKnowledgeGroup();
    else if( const dispatcher::Agent* agent = dynamic_cast< const dispatcher::Agent* >( &owner ) )
        return agent->automat_->GetKnowledgeGroup();
    throw std::runtime_error( __FUNCTION__ );
}
