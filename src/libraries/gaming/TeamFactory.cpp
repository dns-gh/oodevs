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
#include "Team.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamTacticalHierarchies.h"
#include "FormationHierarchy.h"
#include "Formation.h"
#include "StaticModel.h"
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
Team_ABC* TeamFactory::CreateTeam( const MsgsSimToClient::MsgTeamCreation& message )
{
    Team* result = new Team( message, controllers_.controller_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< kernel::Diplomacies_ABC > ( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result ) );
    result->Attach< TacticalHierarchies >     ( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach< IntelligenceHierarchies > ( *new EntityIntelligences    ( controllers_.controller_, *result, 0, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach<kernel::Dotations_ABC>( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Update( message );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
kernel::Formation_ABC* TeamFactory::CreateFormation( const Common::MsgFormationCreation& message )
{
    Entity_ABC* superior = message.has_oid_formation_parente()  ? 
        (Entity_ABC*) &model_.teams_.Resolver< Formation_ABC >::Get( message.oid_formation_parente() ) :
        (Entity_ABC*) &model_.teams_.Resolver< Team_ABC >::Get( message.oid_camp() );

    Formation* result = new Formation( message, controllers_.controller_, model_.static_.levels_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< TacticalHierarchies >    ( *new FormationHierarchy( controllers_.controller_, *result, superior ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, superior, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Dotations_ABC >  ( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Update( message );
    result->Polish();
    return result;
}
