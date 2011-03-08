// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamFactory.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "AggregatedPositions.h"
#include "ConvexHulls.h"
#include "Diplomacies.h"
#include "Dotations.h"
#include "EntityIntelligences.h"
#include "Equipments.h"
#include "Formation.h"
#include "FormationHierarchy.h"
#include "Model.h"
#include "ObjectKnowledges.h"
#include "StaticModel.h"
#include "Team.h"
#include "TeamHierarchies.h"
#include "TeamsModel.h"
#include "TeamTacticalHierarchies.h"
#include "Troops.h"
#include "UrbanKnowledges.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "LogisticLinks.h"
#include "LogisticConsigns.h"
#include "Quotas.h"

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticM  )
    : controllers_( controllers )
    , model_( model )
    , static_( staticM )
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
kernel::Team_ABC* TeamFactory::CreateTeam( const sword::PartyCreation& message )
{
    Team* result = new Team( message, controllers_.controller_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< kernel::Diplomacies_ABC > ( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >     ( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach< kernel::IntelligenceHierarchies > ( *new EntityIntelligences    ( controllers_.controller_, *result, 0, model_.teams_ ) );
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
kernel::Formation_ABC* TeamFactory::CreateFormation( const sword::FormationCreation& message )
{
    kernel::Entity_ABC* superior = message.has_parent()  ?
        (kernel::Entity_ABC*) &model_.teams_.Resolver< kernel::Formation_ABC >::Get( message.parent().id() ) :
        (kernel::Entity_ABC*) &model_.teams_.Resolver< kernel::Team_ABC >::Get( message.party().id() );

    Formation* result = new Formation( message, controllers_.controller_, model_.static_.levels_, static_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::TacticalHierarchies >    ( *new FormationHierarchy( controllers_.controller_, *result, superior ) );
    result->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, superior, model_.teams_ ) );
    result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, result->GetLogisticLevel(), dico ) );
    result->Attach( *new Quotas( controllers_.controller_, static_.objectTypes_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Dotations_ABC >  ( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach< kernel::Positions >( *new AggregatedPositions( *result, 4.f ) );
    result->Update( message );
    result->Polish();
    return result;
}
