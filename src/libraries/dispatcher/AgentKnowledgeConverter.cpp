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
#include "Agent_ABC.h"
#include "AgentKnowledge_ABC.h"
#include "Automat_ABC.h"
#include "KnowledgeGroup_ABC.h"
#include "Model_ABC.h"
#include "PopulationKnowledge.h"
#include <tools/Resolver.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::AgentKnowledgeConverter( const dispatcher::Model_ABC& model )
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
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner ) const
{
    const kernel::Entity_ABC& group = FindGroup( owner );
    tools::Iterator< const dispatcher::AgentKnowledge_ABC& > it = model_.AgentKnowledges().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::AgentKnowledge_ABC& k = it.NextElement();
        if( & k.GetOwner() == &group && k.GetEntity() == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner ) const
{
    const kernel::Entity_ABC& group = FindGroup( owner );
    tools::Iterator< const dispatcher::PopulationKnowledge& > it = model_.PopulationKnowledges().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::PopulationKnowledge& k = it.NextElement();
        if( & k.GetOwner() == &group && k.GetEntity() == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindAgent
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::FindAgent( unsigned long id, const kernel::Entity_ABC& /*owner*/ ) const
{
    return model_.AgentKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindPopulation
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::FindPopulation( unsigned long id, const kernel::Entity_ABC& /*owner*/ ) const
{
    return model_.PopulationKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::AgentKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const kernel::PopulationKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindGroup
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& AgentKnowledgeConverter::FindGroup( const kernel::Entity_ABC& owner ) const
{
    if( const dispatcher::Automat_ABC* automat = dynamic_cast< const dispatcher::Automat_ABC* >( &owner ) )
        return automat->GetKnowledgeGroup();
    else if( const dispatcher::Agent_ABC* agent = dynamic_cast< const dispatcher::Agent_ABC* >( &owner ) )
        return agent->GetSuperior().GetKnowledgeGroup();
    throw MASA_EXCEPTION( "Entity " + owner.GetName().toStdString() + " is neither an agent nor an automat." );
}
