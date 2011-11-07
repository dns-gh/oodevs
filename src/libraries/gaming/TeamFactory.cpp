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
#include "AgentsModel.h"
#include "AggregatedPositions.h"
#include "ConvexHulls.h"
#include "DictionaryExtensions.h"
#include "Diplomacies.h"
#include "Dotations.h"
#include "Equipments.h"
#include "Formation.h"
#include "FormationHierarchy.h"
#include "FormationLives.h"
#include "Lives_ABC.h"
#include "LogisticConsigns.h"
#include "LogisticLinks.h"
#include "Model.h"
#include "ObjectKnowledges.h"
#include "Quotas.h"
#include "StaticModel.h"
#include "StaticModel.h"
#include "Team.h"
#include "TeamHierarchies.h"
#include "TeamTacticalHierarchies.h"
#include "TeamsModel.h"
#include "Troops.h"
#include "TroopsCompatibilityVersion.h"
#include "Color.h"
#include "Symbol.h"
#include "UrbanKnowledges.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticM )
    : controllers_( controllers )
    , model_      ( model )
    , static_     ( staticM )
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
    Team* result = new Team( message, controllers_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< kernel::Diplomacies_ABC > ( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >     ( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach<kernel::Dotations_ABC>( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensionTypes_ ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
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

    Formation* result = new Formation( message, controllers_.controller_, model_.static_.levels_ );
    result->Attach< Lives_ABC >( *new FormationLives( *result ) );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( message.has_symbol() ? message.symbol() : std::string() ) );
    result->Attach< kernel::TacticalHierarchies >( *new FormationHierarchy( controllers_.controller_, *result, superior, model_.symbolsFactory_ ) );
    if( result->GetLogisticLevel() != kernel::LogisticLevel::none_ )
        result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, static_.objectTypes_, result->GetLogisticLevel(), dico, *result ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new LogFuneralConsigns( controllers_.controller_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Dotations_ABC >  ( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach< kernel::Positions >( *new AggregatedPositions( *result, 4.f ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensionTypes_ ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}
