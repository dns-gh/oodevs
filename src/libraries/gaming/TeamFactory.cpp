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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model, const ::StaticModel& staticM )
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
Team_ABC* TeamFactory::CreateTeam( const sword::PartyCreation& message )
{
    Team* result = new Team( message, controllers_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< Diplomacies_ABC > ( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >     ( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< Dotations_ABC>( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::DictionaryExtensions >( *new ::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}

namespace
{
    class NoSideDiplomacy : public Diplomacies_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
         NoSideDiplomacy() {}
        ~NoSideDiplomacy() {}
        //@}

    public:
        //! @name Operations
        //@{
        virtual const Karma& GetDiplomacy( const Entity_ABC& ) const { return Karma::unknown_; } // neutral?
        virtual const Karma& GetKarma() const { return Karma::unknown_; }
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateNoSideTeam
// Created: JSR 2011-11-10
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateNoSideTeam()
{
    Team_ABC* result = new EntityImplementation< Team_ABC >( controllers_.controller_, 0, tools::translate( "TeamFactory", "No side" ) );
    //result->Attach( *new Objects() );
    result->Attach< Diplomacies_ABC >( *new NoSideDiplomacy() );
    result->Attach< kernel::TacticalHierarchies >( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    // TODO other extensions needed?
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation_ABC* TeamFactory::CreateFormation( const sword::FormationCreation& message )
{
    Entity_ABC* superior = message.has_parent()  ?
        (Entity_ABC*) &model_.teams_.Resolver< Formation_ABC >::Get( message.parent().id() ) :
        (Entity_ABC*) &model_.teams_.Resolver< Team_ABC >::Get( message.party().id() );

    Formation* result = new Formation( message, controllers_.controller_, model_.static_.levels_ );
    result->Attach< Lives_ABC >( *new FormationLives( *result ) );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< SymbolHierarchy_ABC >( *new Symbol( message.has_symbol() ? message.symbol() : std::string() ) );
    result->Attach< kernel::TacticalHierarchies >( *new FormationHierarchy( controllers_.controller_, *result, superior, model_.symbolsFactory_ ) );
    if( result->GetLogisticLevel() != LogisticLevel::none_ )
        result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, static_.objectTypes_, result->GetLogisticLevel(), dico, *result ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new LogFuneralConsigns( controllers_.controller_ ) );
    result->Attach( *new Equipments( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< Dotations_ABC >  ( *new Dotations( controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach< Positions >( *new AggregatedPositions( *result, 4.f ) );
    result->Attach< kernel::DictionaryExtensions >( *new ::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}
