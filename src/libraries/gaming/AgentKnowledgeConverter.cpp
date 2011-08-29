// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledgeConverter.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "AgentKnowledges.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::AgentKnowledgeConverter( Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::~AgentKnowledgeConverter()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindAgent
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const AgentKnowledge_ABC* AgentKnowledgeConverter::FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    T_Knowledges::const_iterator it = agents_.find( FindKnowledgeGroup( owner ) );
    if( it == agents_.end() )
        return 0;
    const T_KnowledgeMap& knowledges = it->second;
    for( T_KnowledgeMap::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
        if( it->second && it->second->GetId() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindPopulation
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const PopulationKnowledge_ABC* AgentKnowledgeConverter::FindPopulation( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    T_PopulationKnowledges::const_iterator it = populations_.find( FindKnowledgeGroup( owner ) );
    if( it == populations_.end() )
        return 0;
    const T_PopulationKnowledgeMap& knowledges = it->second;
    for( T_PopulationKnowledgeMap::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
        if( it->second && it->second->GetId() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const AgentKnowledge_ABC& base, const Entity_ABC& owner ) const
{
    const Agent_ABC* real = base.GetEntity();
    if( real )
        return Find( *real, owner );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const AgentKnowledge_ABC* AgentKnowledgeConverter::Find( const Agent_ABC& base, const Entity_ABC& owner ) const
{
    T_Knowledges::const_iterator it = agents_.find( FindKnowledgeGroup( owner ) );
    if( it == agents_.end() )
        return 0;
    T_KnowledgeMap::const_iterator itMap = it->second.find( &base );
    if( itMap == it->second.end() )
        return 0;
    return itMap->second;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const PopulationKnowledge_ABC& base, const Entity_ABC& owner ) const
{
    const Population_ABC* real = base.GetEntity();
    if( real )
        return Find( *real, owner );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge_ABC* AgentKnowledgeConverter::Find( const Population_ABC& base, const Entity_ABC& owner ) const
{
    T_PopulationKnowledges::const_iterator it = populations_.find( FindKnowledgeGroup( owner ) );
    if( it == populations_.end() )
        return 0;
    T_PopulationKnowledgeMap::const_iterator itMap = it->second.find( &base );
    if( itMap == it->second.end() )
        return 0;
    return itMap->second;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::FindKnowledgeGroup
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AgentKnowledgeConverter::FindKnowledgeGroup( const kernel::Entity_ABC& owner ) const
{
    const AgentKnowledges* extension = owner.Retrieve< AgentKnowledges >();
    if( extension )
        return &owner;
    const CommunicationHierarchies* hierarchies = owner.Retrieve< CommunicationHierarchies >();
    if( hierarchies )
    {
        const kernel::Entity_ABC* superior = hierarchies->GetSuperior();
        if( superior )
            return FindKnowledgeGroup( *superior );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyCreated
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyCreated( const AgentKnowledge_ABC& knowledge )
{
    agents_[ & knowledge.GetOwner() ][ knowledge.GetEntity() ] = &knowledge;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyDeleted( const AgentKnowledge_ABC& knowledge )
{
    const kernel::Entity_ABC* owner = & knowledge.GetOwner();
    T_KnowledgeMap& map = agents_[ owner ];
    map.erase( knowledge.GetEntity() );
    if( map.empty() )
        agents_.erase( owner );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyCreated
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyCreated( const PopulationKnowledge_ABC& knowledge )
{
    populations_[ & knowledge.GetOwner() ][ knowledge.GetEntity() ] = &knowledge;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyDeleted( const PopulationKnowledge_ABC& knowledge )
{
    const kernel::Entity_ABC* owner = & knowledge.GetOwner();
    T_PopulationKnowledgeMap& map = populations_[ owner ];
    map.erase( knowledge.GetEntity() );
    if( map.empty() )
        populations_.erase( owner );
}
