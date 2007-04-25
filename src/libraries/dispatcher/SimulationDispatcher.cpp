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
//#include "Publisher_ABC.h"
#include "Model.h"

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
    //    case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_destruction:
    //    case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_destruction:
    //    case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction:
        case T_MsgsSimToClient_msg_msg_population_flux_destruction:
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_destruction:
            return false;
        default:
            return true;
    };
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::OnReceive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SimulationDispatcher::OnReceive( const ASN1T_MsgsSimToClient& asnMsg )
{
    model_.Update( asnMsg );
    if( synching_ && IsNotDestruction( asnMsg ) )
        return;

    clientsPublisher_.Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::Send
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::Send( const ASN1T_MsgsSimToClient& msg )
{
    clientsPublisher_.Send( msg );
    model_.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::StartSynchronisation()
{
    synching_ = true;
    model_.StartSynchronisation( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::EndSynchronisation()
{
    model_.EndSynchronisation();
    synching_ = false;
}
