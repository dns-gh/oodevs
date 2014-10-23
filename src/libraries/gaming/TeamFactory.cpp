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
#include "actions/ActionsModel.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/LogisticHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( kernel::Controllers& controllers,
                          Model& model,
                          const ::StaticModel& staticModel,
                          const kernel::Profile_ABC& profile,
                          actions::ActionsModel& actionsModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , profile_( profile )
    , actionsModel_( actionsModel )
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
    Team* result = new Team( message, controllers_, [=]( const kernel::Team_ABC& team ){ return profile_.CanDoMagic( team ); } );
    result->SetRenameObserver( [=]( const QString& name ){ actionsModel_.PublishRename( *result, name ); } );
    gui::PropertiesDictionary& dico = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new UrbanKnowledges( *result, controllers_.controller_, model_.urbanKnowledgeFactory_ ) );
    result->Attach< kernel::Diplomacies_ABC > ( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamHierarchies        ( controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Attach< kernel::Equipments_ABC >( *new Equipments( *result, controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Dotations_ABC >( *new Dotations( *result,controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::DictionaryExtensions >( *new ::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}

namespace
{
    class NoSideDiplomacy : public kernel::Diplomacies_ABC
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
        virtual const kernel::Karma& GetDiplomacy( const kernel::Entity_ABC& ) const { return kernel::Karma::unknown_; } // neutral?
        virtual const kernel::Karma& GetKarma() const { return kernel::Karma::unknown_; }
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateNoSideTeam
// Created: JSR 2011-11-10
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateNoSideTeam()
{
    auto* result = new gui::EntityImplementation< kernel::Team_ABC >( controllers_.controller_, 0,
                                                                      tools::translate( "TeamFactory", "No side" ),
                                                                      [=]( const kernel::Team_ABC& team ){ return profile_.CanDoMagic( team ); } );
    result->SetRenameObserver( [=]( const QString& name ){ actionsModel_.PublishRename( *result, name ); } );
    result->Attach< kernel::Diplomacies_ABC >( *new NoSideDiplomacy() );
    result->Attach< kernel::TacticalHierarchies >( *new TeamTacticalHierarchies( controllers_.controller_, *result ) );
    result->Update( kernel::InstanciationComplete() );
    // TODO other extensions needed?
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
kernel::Formation_ABC* TeamFactory::CreateFormation( const sword::FormationCreation& message )
{
    kernel::Entity_ABC* superior = message.has_parent() ?
        static_cast< kernel::Entity_ABC*>( &model_.teams_.Resolver< kernel::Formation_ABC >::Get( message.parent().id() ) ) :
        static_cast< kernel::Entity_ABC*>( &model_.teams_.Resolver< kernel::Team_ABC >::Get( message.party().id() ) );

    Formation* result = new Formation( message, controllers_.controller_,
                                       [=]( const kernel::Formation_ABC& formation ){ return profile_.CanDoMagic( formation ); } );
    result->SetRenameObserver( [=]( const QString& name ){ actionsModel_.PublishRename( *result, name ); } );
    result->Attach< Lives_ABC >( *new FormationLives( *result ) );
    gui::PropertiesDictionary& dico = result->Get< gui::PropertiesDictionary >();
    std::string symbol = message.has_app6symbol() ? message.app6symbol() : std::string();
    const kernel::Karma& karma = superior->Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    symbol = symbol.empty() ? model_.symbolsFactory_.GetSymbolBase( karma ) : symbol;
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbol, model_.symbolsFactory_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new FormationHierarchy( controllers_.controller_, *result, superior, model_.symbolsFactory_, model_.GetFormationResolver(), model_.GetTeamResolver() ) );
    result->Attach( *new gui::LogisticBase( controllers_, *result, dico, true, message.logistic_level() == sword::logistic_base, false ) );
    if( result->Get< gui::LogisticBase >().IsBase() )
        result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, static_.objectTypes_, dico, *result ) );
    result->Attach( *new LogMaintenanceConsigns( controllers_.controller_ ) );
    result->Attach( *new LogMedicalConsigns( controllers_.controller_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new LogFuneralConsigns( controllers_.controller_ ) );
    result->Attach< kernel::Equipments_ABC >( *new Equipments( *result,controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Dotations_ABC >( *new Dotations( *result, controllers_.controller_, model_.static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach< kernel::Positions >( *new AggregatedPositions( *result ) );
    result->Attach< kernel::DictionaryExtensions >( *new ::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< gui::LogisticHierarchiesBase >( *new kernel::LogisticHierarchies( controllers_.controller_, *result, model_.GetAutomatResolver(), model_.GetFormationResolver() ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}
