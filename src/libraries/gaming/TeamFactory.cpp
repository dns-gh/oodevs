// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "ObjectKnowledges.h"
#include "AgentKnowledges.h"
#include "PopulationKnowledges.h"
#include "KnowledgeGroup.h"
#include "Team.h"
#include "Diplomacies.h"
#include "AgentKnowledgeFactory.h"
#include "KnowledgeGroupHierarchies.h"
#include "TeamHierarchies.h"
#include "TeamTacticalHierarchies.h"
#include "FormationHierarchy.h"
#include "Formation.h"
#include "StaticModel.h"
#include "KnowledgeGroupsModel.h"
#include "Equipments.h"
#include "Troops.h"
#include "Dotations.h"
#include "ConvexHulls.h"
#include "EntityIntelligences.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "UrbanKnowledges.h"
#include "clients_kernel/KnowledgeGroupType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::~TeamFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam( const ASN1T_MsgTeamCreation& asnMsg )
{
    Team* result = new Team( asnMsg, controllers_.controller_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< kernel::Diplomacies_ABC >( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result, *this ) );
    result->Attach< TacticalHierarchies >     ( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach< IntelligenceHierarchies > ( *new EntityIntelligences    ( controllers_.controller_, *result, 0, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Update( asnMsg );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
kernel::Formation_ABC* TeamFactory::CreateFormation( const ASN1T_MsgFormationCreation& asnMsg )
{
    Entity_ABC* superior = asnMsg.m.oid_formation_parentePresent ? 
        (Entity_ABC*) &model_.teams_.Resolver< Formation_ABC >::Get( asnMsg.oid_formation_parente ) :
        (Entity_ABC*) &model_.teams_.Resolver< Team_ABC >::Get( asnMsg.oid_camp );

    Formation* result = new Formation( asnMsg, controllers_.controller_, model_.static_.levels_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< TacticalHierarchies >( *new FormationHierarchy( controllers_.controller_, *result, superior ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, superior, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Update( asnMsg );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( const ASN1T_MsgKnowledgeGroupCreation& asnMsg, Team_ABC& team  )
{
    Entity_ABC* superior = asnMsg.m.oid_knowledgegroup_parentPresent ? 
        (Entity_ABC*) &model_.knowledgeGroups_.Resolver< KnowledgeGroup_ABC >::Get( asnMsg.oid_knowledgegroup_parent ) :
        (Entity_ABC*) &model_.teams_.Resolver< Team_ABC >::Get( asnMsg.oid_camp );

    KnowledgeGroup* result = new KnowledgeGroup( asnMsg.oid, controllers_.controller_, asnMsg.type );
    result->Attach( *new AgentKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    result->Attach( *new PopulationKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    result->Attach< CommunicationHierarchies >( *new KnowledgeGroupHierarchies( controllers_.controller_, superior, *result, model_.knowledgeGroups_ ) );
    result->Polish();

    // $$$$ AGE 2006-10-24: 
    model_.knowledgeGroups_.Register( result->GetId(), *result );
    return result;
}
