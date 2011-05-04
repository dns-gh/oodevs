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
#include "Model.h"
#include "Agent.h"
#include "Automat.h"
#include "Population.h"
#include "Inhabitant.h"
#include "Affinities.h"
#include "InhabitantHierarchies.h"
#include "InhabitantPositions.h"
#include "Attributes.h"
#include "Contaminations.h"
#include "DebugPoints.h"
#include "Dotations.h"
#include "Equipments.h"
#include "HumanFactors.h"
#include "Borrowings.h"
#include "Lendings.h"
#include "MissionParameters.h"
#include "LogisticLinks.h"
#include "Paths.h"
#include "Reinforcements.h"
#include "Reports.h"
#include "Transports.h"
#include "Troops.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "SupplyStates.h"
#include "ObjectDetections.h"
#include "AgentDetections.h"
#include "VisionCones.h"
#include "LivingArea.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticsModel.h"
#include "LimitsModel.h"
#include "AgentFactory.h"
#include "PopulationDetections.h"
#include "LogisticConsigns.h"
#include "Logistics.h"
#include "Explosions.h"
#include "Decisions.h"
#include "AutomatDecisions.h"
#include "Fires.h"
#include "AgentPositions.h"
#include "PopulationPositions.h"
#include "Lives.h"
#include "Lives_ABC.h"
#include "PopulationDecisions.h"
#include "MagicOrders.h"
#include "StaticModel.h"
#include "AgentHierarchies.h"
#include "AutomatLives.h"
#include "AggregatedPositions.h"
#include "AutomatHierarchies.h"
#include "AutomatTacticalHierarchies.h"
#include "PopulationHierarchies.h"
#include "PcAttributes.h"
#include "ConvexHulls.h"
#include "AgentConvexHulls.h"
#include "DecisionalStates.h"
#include "Speeds.h"
#include "Weapons.h"
#include "Quotas.h"
#include "UrbanPerceptions.h"
#include "AgentHierarchiesCommunication.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommandPostAttributes.h"
#include "clients_kernel/Formation_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                            Publisher_ABC& publisher, kernel::Workers& workers, const RcEntityResolver_ABC& rcResolver )
    : controllers_( controllers )
    , model_      ( model )
    , static_     ( staticModel )
    , publisher_  ( publisher )
    , workers_    ( workers )
    , rcResolver_ ( rcResolver )
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
    Automat* result = new Automat( message, controllers_.controller_, static_.types_, static_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();

    result->Attach< kernel::CommunicationHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, model_.knowledgeGroups_, dico ) );
    kernel::Entity_ABC* superior = 0;

    if( message.parent().has_formation() )
        superior = & (( tools::Resolver< kernel::Formation_ABC >&)( model_.teams_ )) .Get( message.parent().formation().id() );
    else
        superior = & (( tools::Resolver< kernel::Automat_ABC >&)  ( model_.agents_ )).Get( message.parent().automat().id() );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatTacticalHierarchies( controllers_.controller_, *result, *superior, model_.agents_, model_.teams_ ) );
    result->Attach< Lives_ABC >( *new AutomatLives( *result ) );
    result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, model_.teams_, result->GetLogisticLevel(), dico ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, publisher_, *result ) );
    result->Attach< kernel::Positions >( *new AggregatedPositions( *result, 2.f ) );
    result->Attach( *new Logistics( *result, controllers_.controller_, model_, static_, dico ) );
    result->Attach( *new Quotas( controllers_.controller_, static_.objectTypes_ ) );
    result->Attach( *new LogMaintenanceConsigns( controllers_.controller_ ) );
    result->Attach( *new LogMedicalConsigns( controllers_.controller_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach( *new Reports( *result, controllers_.controller_, static_.reportFactory_ ) );
    result->Attach<kernel::Dotations_ABC>( *new Dotations( controllers_.controller_, static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new MissionParameters( controllers_.controller_, model_.actionFactory_ ) );
    result->Attach( *new DebugPoints( static_.coordinateConverter_ ) );
    result->Attach( *new ConvexHulls( *result ) );
    result->Attach( *new DecisionalStates() );

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
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();

    result->Attach< Lives_ABC >( *new Lives( controllers_.controller_ ) );
    result->Attach< kernel::Attributes_ABC >( *new Attributes( controllers_.controller_, static_.coordinateConverter_, dico, model_.teams_ ) );
    result->Attach( *new kernel::CommandPostAttributes( *result ) );
    result->Attach( *new Decisions( controllers_.controller_, *result ) );
    result->Attach< kernel::Positions >( *new AgentPositions( controllers_.controller_, *result, static_.coordinateConverter_ ) );
    result->Attach( *new VisionCones( *result, model_.surfaceFactory_, workers_ ) );
    result->Attach( *new AgentDetections( controllers_.controller_, model_.agents_, *result ) );
    result->Attach( *new MagicOrders( *result ) );
    result->Attach( *new Logistics( *result, controllers_.controller_, model_, static_, dico ) );
    result->Attach( *new LogMaintenanceConsigns( controllers_.controller_ ) );
    result->Attach( *new LogMedicalConsigns( controllers_.controller_ ) );
    result->Attach( *new LogSupplyConsigns( controllers_.controller_ ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AgentHierarchiesCommunication( controllers_.controller_, *result, model_.agents_, model_.knowledgeGroups_ ) );
    result->Attach< kernel::TacticalHierarchies >     ( *new AgentHierarchies< kernel::TacticalHierarchies >     ( controllers_.controller_, *result, model_.agents_ ) );
    if( result->IsCommandPost() )
        result->Attach( *new PcAttributes( *result ) );

    result->Attach< kernel::HumanFactors_ABC >( *new HumanFactors( controllers_.controller_, dico ) );
    result->Attach( *new Reinforcements( controllers_.controller_, model_.agents_, dico ) );
    result->Attach<kernel::Dotations_ABC>( *new Dotations( controllers_.controller_, static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Equipments( controllers_.controller_, static_.objectTypes_, dico, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Lendings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    result->Attach( *new Borrowings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    result->Attach( *new Transports( controllers_.controller_, model_.agents_, dico ) );
    result->Attach( *new Troops( controllers_.controller_, model_.agents_, model_.teams_, model_.teams_ ) );
    result->Attach( *new Contaminations( controllers_.controller_, static_.objectTypes_, dico ) );
    result->Attach< ConvexHulls >( *new AgentConvexHulls( *result, static_.coordinateConverter_ ) );
    result->Attach( *new DecisionalStates() );
    result->Attach( *new Speeds( static_.coordinateConverter_ ) );
    result->Attach( *new Weapons( controllers_, static_.objectTypes_, static_.objectTypes_ ) );
    result->Attach( *new Affinities( controllers_.controller_, model_.teams_, dico ) );
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
    Population* result = new Population( message, controllers_, static_.coordinateConverter_, static_.types_, static_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new PopulationPositions( *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, model_.teams_.GetTeam( message.party().id() ) ) );
    result->Attach( *new PopulationDecisions( controllers_.controller_, *result ) );
    result->Attach( *new DecisionalStates() );
    result->Attach( *new Affinities( controllers_.controller_, model_.teams_, dico ) );
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
    Inhabitant* result = new Inhabitant( message, controllers_, model_.urbanObjects_, static_.types_, static_.objectTypes_, static_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new InhabitantPositions( *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, model_.teams_.GetTeam( message.party().id() ) ) );
    result->Attach( *new Affinities( controllers_.controller_, model_.teams_, dico ) );
    result->Attach< kernel::LivingArea_ABC >( *new LivingArea( message, result->GetId(), controllers_.controller_, model_.urbanObjects_ ) );
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
    agent.Attach( *new MissionParameters( controllers_.controller_, model_.actionFactory_ ) );
    agent.Attach( *new Paths( static_.coordinateConverter_ ) );
    agent.Attach( *new Reports( agent, controllers_.controller_, static_.reportFactory_ ) );
    agent.Attach( *new ObjectDetections( controllers_.controller_, model_.objects_ ) );
    agent.Attach( *new PopulationDetections( controllers_.controller_, static_.coordinateConverter_, model_.agents_, agent ) );
    agent.Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    agent.Attach( *new Fires( controllers_.controller_, model_.fireFactory_, agent.GetId() ) );
    agent.Attach( *new UrbanPerceptions( controllers_.controller_, model_.agents_ ) );
}
