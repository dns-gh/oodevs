// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentFactory.h"
#include "Affinities.h"
#include "Agent.h"
#include "AgentConvexHulls.h"
#include "AgentDecisions.h"
#include "AgentDetections.h"
#include "AgentHierarchiesCommunication.h"
#include "AgentPositions.h"
#include "AgentsModel.h"
#include "AggregatedPositions.h"
#include "Attributes.h"
#include "Automat.h"
#include "AutomatHierarchies.h"
#include "AutomatLives.h"
#include "AutomatTacticalHierarchies.h"
#include "Borrowings.h"
#include "Contaminations.h"
#include "ConvexHulls.h"
#include "DebugPoints.h"
#include "DecisionalStates.h"
#include "DictionaryExtensions.h"
#include "Direction.h"
#include "Dotations.h"
#include "Equipments.h"
#include "Explosions.h"
#include "Fires.h"
#include "HumanFactors.h"
#include "Inhabitant.h"
#include "InhabitantHierarchies.h"
#include "InhabitantPositions.h"
#include "KnowledgeGroupsModel.h"
#include "Lendings.h"
#include "Lives.h"
#include "LivingArea.h"
#include "LogisticConsigns.h"
#include "LogisticLinks.h"
#include "Logistics.h"
#include "MissionParameters.h"
#include "Model.h"
#include "ObjectDetections.h"
#include "ObjectsModel.h"
#include "PointingKnowledges.h"
#include "Population.h"
#include "PopulationDecisions.h"
#include "PopulationDetections.h"
#include "PopulationHierarchies.h"
#include "PopulationHierarchiesCommunication.h"
#include "PopulationPositions.h"
#include "Reinforcements.h"
#include "Reports.h"
#include "StaticModel.h"
#include "Symbol.h"
#include "TeamsModel.h"
#include "Transports.h"
#include "Troops.h"
#include "TroopsCompatibilityVersion.h"
#include "UrbanPerceptions.h"
#include "VisionCones.h"
#include "Weapons.h"
#include "Color.h"
#include "CommandPostAttributes.h"

#include "clients_gui/AutomatDecisions.h"
#include "clients_gui/CriticalIntelligence.h"
#include "clients_gui/EntityType.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/Paths.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                            Publisher_ABC& publisher, kernel::Workers& workers )
    : controllers_( controllers )
    , model_      ( model )
    , static_     ( staticModel )
    , publisher_  ( publisher )
    , workers_    ( workers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( const sword::AutomatCreation& message )
{
    const kernel::AutomatType* type = static_.types_.tools::Resolver< kernel::AutomatType >::Find( message.type().id() );
    if( !type )
        return 0;
    Automat* result = new Automat( message, controllers_.controller_, *type );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::AutomatType >( *result, *type, dictionary ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, model_.knowledgeGroups_, dictionary ) );
    kernel::Entity_ABC* superior = 0;

    if( message.parent().has_formation() )
        superior = &model_.GetFormationResolver().Get( message.parent().formation().id() );
    else
        superior = &model_.GetAutomatResolver().Get( message.parent().automat().id() );
    std::string symbol = message.has_app6symbol() ? message.app6symbol() : std::string();
    const kernel::Karma& karma = superior->Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    symbol = symbol.empty() ? model_.symbolsFactory_.GetSymbolBase( karma ) : symbol;
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbol, model_.symbolsFactory_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatTacticalHierarchies( controllers_.controller_, *result, *superior, model_.agents_, model_.teams_ ) );
    result->Attach< Lives_ABC >( *new AutomatLives( *result ) );
    result->Attach( *new gui::LogisticBase( controllers_, *result, dictionary, type->IsTC2(), message.logistic_level() == sword::logistic_base ) );
    result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, static_.objectTypes_, dictionary, *result ) );
    result->Attach< gui::Decisions_ABC >( *new gui::AutomatDecisions( controllers_.controller_, *result, static_.types_.automatModels_ ) );
    result->Attach< kernel::Positions >( *new AggregatedPositions( *result, 2.f ) );
    result->Attach( *new Logistics( *result, controllers_.controller_, model_, static_, dictionary ) );
    result->Attach( *new LogMaintenanceConsigns( controllers_.controller_ ) );
    result->Attach( *new LogMedicalConsigns( controllers_.controller_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new LogFuneralConsigns( controllers_.controller_ ) );
    result->Attach( *new Reports( *result, controllers_.controller_, static_.reportFactory_ ) );
    result->Attach< kernel::Dotations_ABC >( *new Dotations( *result,controllers_.controller_, static_.objectTypes_, dictionary, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Equipments_ABC >( *new Equipments( *result,controllers_.controller_, static_.objectTypes_, dictionary, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new MissionParameters( controllers_.controller_, model_.actionFactory_, result->GetId() ) );
    result->Attach( *new DebugPoints( static_.coordinateConverter_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach( *new DecisionalStates( *result ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< gui::LogisticHierarchiesBase >( *new kernel::LogisticHierarchies( controllers_.controller_, *result, model_.GetAutomatResolver(), model_.GetFormationResolver() ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( const sword::UnitCreation& message )
{
    Agent* result = new Agent( message, controllers_.controller_, static_.types_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( *result, controllers_.controller_, dictionary ) );
    result->Attach< Lives_ABC >( *new Lives( *result, controllers_.controller_ ) );
    result->Attach( *new Direction( *result ) );
    result->Attach< kernel::CommandPostAttributes_ABC >( *new CommandPostAttributes( *result, message, static_.types_ ) ); // $$$$ LDC Warning: Must be before new Attributes because Attributes uses it without knowing it to paint the headquarters symbol...
    result->Attach( *new Attributes( *result, controllers_.controller_, static_.detection_, static_.coordinateConverter_, dictionary, model_.teams_ ) );
    result->Attach< gui::Decisions_ABC >( *new AgentDecisions( controllers_.controller_, *result, static_.types_.unitModels_ ) );
    result->Attach< kernel::Positions >( *new AgentPositions( controllers_.controller_, *result, static_.coordinateConverter_ ) );
    result->Attach( *new VisionCones( *result, model_, workers_, controllers_.controller_ ) );
    result->Attach( *new AgentDetections( controllers_.controller_, model_.agents_, *result ) );
    result->Attach( *new Logistics( *result, controllers_.controller_, model_, static_, dictionary ) );
    result->Attach( *new LogMaintenanceConsigns( controllers_.controller_ ) );
    result->Attach( *new LogMedicalConsigns( controllers_.controller_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new LogFuneralConsigns( controllers_.controller_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AgentHierarchiesCommunication( controllers_, *result, model_.agents_, model_.knowledgeGroups_ ) );
    result->Attach< kernel::TacticalHierarchies >     ( *new AgentHierarchies< kernel::TacticalHierarchies >     ( controllers_.controller_, *result, model_.agents_ ) );
    result->Attach< kernel::HumanFactors_ABC >( *new HumanFactors( *result, controllers_.controller_, dictionary ) );
    result->Attach( *new Reinforcements( *result, controllers_.controller_, model_.agents_, dictionary ) );
    result->Attach< kernel::Dotations_ABC >( *new Dotations( *result,controllers_.controller_, static_.objectTypes_, dictionary, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach< kernel::Equipments_ABC >( *new Equipments( *result,controllers_.controller_, static_.objectTypes_, dictionary, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Lendings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    result->Attach( *new Borrowings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    result->Attach( *new Transports( *result, controllers_.controller_, model_.agents_, dictionary ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new TroopsCompatibilityVersion( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Contaminations( controllers_.controller_, *result, static_.objectTypes_, dictionary, result->GetType() ) );
    result->Attach< ConvexHulls >( *new AgentConvexHulls( *result, static_.coordinateConverter_ ) );
    result->Attach( *new DecisionalStates( *result ) );
    result->Attach( *new Weapons( controllers_, static_.objectTypes_, static_.objectTypes_ ) );
    result->Attach( *new Affinities( *result, controllers_.controller_, model_.teams_, dictionary ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach( *new PointingKnowledges() );
    AttachExtensions( *result );

    result->Update( message );
    result->Polish();

    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( const sword::CrowdCreation& message )
{
    const kernel::PopulationType* type = static_.types_.tools::Resolver< kernel::PopulationType >::Find( message.type().id() );
    if( !type )
        return 0;
    const kernel::Team_ABC& team = model_.teams_.GetTeam( message.party().id() );
    Population* result = new Population( message, controllers_, static_.coordinateConverter_, *type );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach< kernel::CommunicationHierarchies >( *new PopulationHierarchiesCommunication( controllers_.controller_, *result, model_.knowledgeGroups_.FindCrowdKnowledgeGroup( team ) ) );
    result->Attach( *new gui::EntityType< kernel::PopulationType >( *result, *type, dictionary ) );
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( *result, controllers_.controller_, dictionary ) );
    result->Attach< kernel::Positions >( *new PopulationPositions( *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, team ) );
    result->Attach< gui::Decisions_ABC >( *new PopulationDecisions( controllers_.controller_, *result, *type ) );
    result->Attach( *new DecisionalStates( *result ) );
    result->Attach( *new Affinities( *result, controllers_.controller_, model_.teams_, dictionary ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    AttachExtensions( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* AgentFactory::Create( const sword::PopulationCreation& message )
{
    const kernel::InhabitantType& type = static_.types_.tools::Resolver< kernel::InhabitantType >::Get( message.type().id() );
    Inhabitant* result = new Inhabitant( message, controllers_.controller_, model_.urbanObjects_, type, static_.objectTypes_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::InhabitantType >( *result, type, dictionary ) );
    result->Attach< kernel::Positions >( *new InhabitantPositions( *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, model_.teams_.GetTeam( message.party().id() ) ) );
    result->Attach( *new Affinities( *result, controllers_.controller_, model_.teams_, dictionary ) );
    result->Attach< kernel::LivingArea_ABC >( *new LivingArea( message, result->GetId(), controllers_.controller_, model_.urbanObjects_ ) );
    result->Attach< kernel::DictionaryExtensions >( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::AttachExtensions
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::AttachExtensions( kernel::Entity_ABC& agent )
{
    agent.Attach( *new DebugPoints( static_.coordinateConverter_ ) );
    agent.Attach( *new MissionParameters( controllers_.controller_, model_.actionFactory_, agent.GetId() ) );
    agent.Attach( *new gui::Paths( static_.coordinateConverter_ ) );
    agent.Attach( *new Reports( agent, controllers_.controller_, static_.reportFactory_ ) );
    agent.Attach( *new ObjectDetections( controllers_.controller_, model_.objects_ ) );
    agent.Attach( *new PopulationDetections( controllers_.controller_, static_.coordinateConverter_, model_.agents_, agent ) );
    agent.Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    agent.Attach( *new Fires( controllers_.controller_, model_.fireFactory_, agent.GetId() ) );
    agent.Attach( *new UrbanPerceptions( controllers_.controller_, model_.agents_ ) );
}
