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
#include "Lends.h"
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
#include "Hierarchies.h"
#include "ObjectDetections.h"
#include "AgentDetections.h"
#include "VisionCones.h"

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controller& controller, AgentTypes& types )
    : controller_( controller )
    , model_( 0 )
    , types_( types )
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
// Name: AgentFactory::SetModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::SetModel( Model& model )
{
    model_ = &model;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* AgentFactory::Create( const ASN1T_MsgAutomateCreation& asnMsg )
{
    Agent* result = new Agent( asnMsg, types_ );
    AttachExtensions( *result );
    result->Update( asnMsg );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* AgentFactory::Create( const ASN1T_MsgPionCreation& asnMsg )
{
    Agent* result = new Agent( asnMsg, types_ );
    AttachExtensions( *result );
    result->Update( asnMsg );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population* AgentFactory::Create( const ASN1T_MsgPopulationCreation& asnMsg )
{
    Population* result = new Population( asnMsg );
    AttachExtensions( *result );
//    result->Update( asnMsg );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::AttachExtensions
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::AttachExtensions( Agent_ABC& agent )
{
    agent.Attach( *new Attributes( controller_ ) );
    agent.Attach( *new Contaminations( controller_ ) );
    agent.Attach( *new DebugPoints() );
    agent.Attach( *new Dotations( controller_ ) );
    agent.Attach( *new Equipments( controller_ ) );
    agent.Attach( *new HumanFactors( controller_ ) );
    agent.Attach( *new Lends( controller_, *model_ ) );
    agent.Attach( *new Limits( *model_ ) );
    agent.Attach( *new LogisticLinks( controller_, *model_ ) );
    agent.Attach( *new Paths() );
    agent.Attach( *new Reinforcements( controller_, *model_ ) );
    agent.Attach( *new Reports( agent, controller_ ) );
    agent.Attach( *new Transports( controller_, *model_ ) );
    agent.Attach( *new Troops( controller_ ) );
    agent.Attach( *new MaintenanceStates( controller_ ) );
    agent.Attach( *new MedicalStates( controller_ ) );
    agent.Attach( *new SupplyStates( controller_ ) );
    agent.Attach( *new Hierarchies( controller_, *model_, *model_ ) );
    agent.Attach( *new ObjectDetections( controller_, *model_ ) );
    agent.Attach( *new AgentDetections( controller_, *model_ ) );
    agent.Attach( *new VisionCones() );
}

