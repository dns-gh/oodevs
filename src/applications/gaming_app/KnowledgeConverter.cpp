// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "KnowledgeConverter.h"
#include "gaming/AgentKnowledges.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeConverter constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
KnowledgeConverter::KnowledgeConverter( Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeConverter destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
KnowledgeConverter::~KnowledgeConverter()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeConverter::FindAgent
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const AgentKnowledge_ABC* KnowledgeConverter::FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    auto it = agents_.find( FindKnowledgeGroup( owner ) );
    if( it == agents_.end() )
        return 0;
    const T_KnowledgeMap& knowledges = it->second;
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
        // The identifier received is actually the one from the agent and
        // not the knowledge identifier.
        if( it->second && it->second->GetEntityId() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeConverter::FindKnowledgeGroup
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* KnowledgeConverter::FindKnowledgeGroup( const kernel::Entity_ABC& owner ) const
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
// Name: KnowledgeConverter::NotifyCreated
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void KnowledgeConverter::NotifyCreated( const AgentKnowledge_ABC& knowledge )
{
    agents_[ &knowledge.GetOwner() ][ knowledge.GetEntity() ] = &knowledge;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeConverter::NotifyDeleted
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void KnowledgeConverter::NotifyDeleted( const AgentKnowledge_ABC& knowledge )
{
    const kernel::Entity_ABC* owner = & knowledge.GetOwner();
    auto& map = agents_[ owner ];
    map.erase( knowledge.GetEntity() );
    if( map.empty() )
        agents_.erase( owner );
}
