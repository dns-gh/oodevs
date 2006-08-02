// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentFactory.h"

#include "Model.h"
#include "AgentTypes.h"
#include "Agent.h"
#include "Population.h"

#include "Attributes.h"
#include "Contaminations.h"
#include "DebugPoints.h"
#include "Dotations.h"
#include "Equipments.h"
#include "HumanFactors.h"
#include "Borrowings.h"
#include "Lendings.h"
#include "Limits.h"
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
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticsModel.h"
#include "LimitsModel.h"
#include "AgentFactory.h"
#include "ObjectFactory.h"
#include "AgentTypes.h"
#include "ObjectTypes.h"
#include "PopulationDetections.h"
#include "LogisticConsigns.h"
#include "Logistics.h"
#include "Explosions.h"
#include "Decisions.h"
#include "AutomatDecisions.h"
#include "Fires.h"
#include "AgentPositions.h"
#include "PopulationPositions.h"
#include "Controllers.h"
#include "Lives.h"
#include "PopulationDecisions.h"
#include "MagicOrders.h"
#include "CoordinateConverter.h"
#include "DataDictionary.h"
#include "StaticModel.h"

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, Publisher_ABC& publisher, const Simulation& simulation, Workers& workers )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , publisher_( publisher )
    , simulation_( simulation )
    , workers_( workers )
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

// $$$$ AGE 2006-04-10: C'est l'ordre d'attach qui impose l'ordre de dessin. 
// $$$$ AGE 2006-04-10: C'est pas terrible
// $$$$ AGE 2006-08-01: Faire un drawer qui parcours les extensions, les enregistre et détermine un ordre

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* AgentFactory::Create( const ASN1T_MsgAutomateCreation& asnMsg )
{
    Agent* result = new Agent( asnMsg, controllers_.controller_, static_.types_, model_.agents_, model_.knowledgeGroups_ );
    DataDictionary& dico = result->Get< DataDictionary >();
    result->Attach( *new Lives( *result ) );
    result->InterfaceContainer< Extension_ABC >::Register( *result );
    result->Attach( *new Attributes( controllers_.controller_, static_.coordinateConverter_, result->Get< DataDictionary >() ) );
    AttachExtensions( *result );
    result->Attach( *new LogisticLinks( controllers_.controller_, model_.agents_, *result->GetAutomatType(), dico ) );
    result->Attach( *new Decisions( controllers_.controller_, *result ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, publisher_, *result ) );
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_ ) );
    result->Attach( *new VisionCones( *result, static_.surfaceFactory_, workers_ ) );
    result->Attach( *new AgentDetections( controllers_.controller_, model_.agents_, result->GetTeam() ) );
    result->Attach( *new MagicOrders( controllers_.controller_, *result ) );
    result->Update( asnMsg );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* AgentFactory::Create( const ASN1T_MsgPionCreation& asnMsg )
{
    Agent* result = new Agent( asnMsg, controllers_.controller_, static_.types_, model_.agents_, model_.knowledgeGroups_ );
    result->Attach( *new Lives( *result ) );
    result->InterfaceContainer< Extension_ABC >::Register( *result );
    result->Attach( *new Attributes( controllers_.controller_, static_.coordinateConverter_, result->Get< DataDictionary >() ) );
    AttachExtensions( *result );
    result->Attach( *new Decisions( controllers_.controller_, *result ) );
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_ ) );
    result->Attach( *new VisionCones( *result, static_.surfaceFactory_, workers_ ) );
    result->Attach( *new AgentDetections( controllers_.controller_, model_.agents_, result->GetTeam() ) );
    result->Attach( *new MagicOrders( controllers_.controller_, *result ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population* AgentFactory::Create( const ASN1T_MsgPopulationCreation& asnMsg )
{
    Population* result = new Population( asnMsg, controllers_.controller_, static_.coordinateConverter_, model_.teams_, static_.types_ );
    AttachExtensions( *result ); // $$$$ AGE 2006-02-16: pas tout !
    result->Attach< Positions >( *new PopulationPositions( *result ) );
    result->Attach( *new PopulationDecisions( *result ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::AttachExtensions
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::AttachExtensions( Agent_ABC& agent )
{
    DataDictionary& dico = agent.Get< DataDictionary >();
    agent.Attach( *new Contaminations( controllers_.controller_, static_.objectTypes_, dico ) );
    agent.Attach( *new DebugPoints() );
    agent.Attach( *new Dotations( controllers_.controller_, static_.objectTypes_, agent.Get< DataDictionary >() ) );
    agent.Attach( *new Equipments( controllers_.controller_, static_.objectTypes_, dico ) );
    agent.Attach( *new HumanFactors( controllers_.controller_, dico ) );
    agent.Attach( *new Lendings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    agent.Attach( *new Borrowings( controllers_.controller_, model_.agents_, static_.objectTypes_ ) );
    agent.Attach( *new Limits( model_.limits_ ) );
    agent.Attach( *new Paths( static_.coordinateConverter_ ) );
    agent.Attach( *new Reinforcements( controllers_.controller_, model_.agents_, dico ) );
    agent.Attach( *new Reports( agent, controllers_.controller_, simulation_ ) );
    agent.Attach( *new Transports( controllers_.controller_, model_.agents_, dico ) );
    agent.Attach( *new Troops( controllers_.controller_ ) );
    agent.Attach( *new Logistics( agent, controllers_.controller_, model_, static_, dico ) );
    agent.Attach( *new ObjectDetections( controllers_.controller_, model_.objects_ ) );
    agent.Attach( *new PopulationDetections( controllers_.controller_, model_.agents_ ) );
    agent.Attach( *new LogisticConsigns( controllers_.controller_ ) );
    agent.Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    agent.Attach( *new Fires( controllers_.controller_, model_.fireFactory_ ) );
}

