// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationDispatcher.h"
#include "Model.h"
#include "Publisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::SimulationDispatcher( Publisher_ABC& clientsPublisher, Model& model )
    : clientsPublisher_( clientsPublisher )
    , model_           ( model )
    , synching_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::~SimulationDispatcher()
{
    // NOTHING
}


// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::IsNotDestruction
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool SimulationDispatcher::IsNotDestruction( const ASN1T_MsgsSimToClient& asnMsg ) const
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_limit_destruction:
        case T_MsgsSimToClient_msg_msg_lima_destruction:
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_object_destruction:
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_log_medical_handling_destruction:
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction:
        case T_MsgsSimToClient_msg_msg_log_supply_handling_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction:
        case T_MsgsSimToClient_msg_msg_population_flow_destruction:
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction:
            return false;
        default:
            return true;
    };
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::Receive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SimulationDispatcher::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( synching_ && IsNotDestruction( asnMsg ) )
        return;

    clientsPublisher_.Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::StartSynchronisation()
{
    synching_ = true;
    model_.StartSynchronisation();
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::EndSynchronisation()
{
    model_.EndSynchronisation( clientsPublisher_ );
    synching_ = false;
}
