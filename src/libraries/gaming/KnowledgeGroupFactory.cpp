// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupFactory.h"
#include "AgentKnowledges.h"
#include "AgentKnowledgeFactory.h"
#include "KnowledgeGroup.h"
#include "KnowledgeGroupHierarchies.h"
#include "KnowledgeGroupsModel.h"
#include "Model.h"
#include "ObjectKnowledges.h"
#include "PopulationKnowledges.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory constructor
// Created: HBD 2010-06-23
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::KnowledgeGroupFactory( kernel::Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory destructor
// Created: HBD 2010-06-23
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::~KnowledgeGroupFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::CreateKnowledgeGroup
// Created: HBD 2010-06-23
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::CreateKnowledgeGroup( const sword::KnowledgeGroupCreation& message )
{
    // LTO begin
    Entity_ABC* superior = message.has_parent() ?
        static_cast< Entity_ABC* >( &model_.GetKnowledgeGroupResolver().Get( message.parent().id() ) ) :
        static_cast< Entity_ABC* >( &model_.GetTeamResolver().Get( message.party().id() ) );
    // LTO end

    KnowledgeGroup* result = new KnowledgeGroup( message.knowledge_group().id(), controllers_.controller_, message.type(), model_.static_.types_ );
    result->Attach( *new AgentKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    bool jam = message.has_jam() && message.jam();
    if( jam )
        result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new PopulationKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    result->Attach< CommunicationHierarchies >( *new KnowledgeGroupHierarchies( controllers_.controller_, superior, *result, model_.GetKnowledgeGroupResolver(), jam ) ); // LTO
    result->Polish();
    return result;
}
