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
#include "Entity_ABC.h"
#include "Synchroniser.h"
#include "ReplaySynchronisations.h"
#include "ModelVisitor_ABC.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::SimulationDispatcher( ClientPublisher_ABC& clientsPublisher, Model& model )
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
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:
        case T_MsgsSimToClient_msg_msg_stop_unit_fire:
        case T_MsgsSimToClient_msg_msg_stop_population_fire:
        case T_MsgsSimToClient_msg_msg_invalidate_report:
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

namespace
{
    class StartSynchVisitor : public ModelVisitor_ABC
    {
    public:
        virtual void Visit( Entity_ABC& entity )
        {
            entity.Get< ReplaySynchronisations >().StartSynchronisation( false );
        }
    };

    class EndSynchVisitor : public ModelVisitor_ABC
    {
    public:
        EndSynchVisitor( Synchroniser& model )
            : model_( &model ) {}
        virtual void Visit( Entity_ABC& entity )
        {
            entity.Get< ReplaySynchronisations >().EndSynchronisation( *model_ );
        }
    private:
        Synchroniser* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::StartSynchronisation()
{
    synching_ = true;
    StartSynchVisitor visitor;
    model_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::EndSynchronisation()
{
    Synchroniser synch;
    EndSynchVisitor visitor( synch );
    model_.Accept( visitor );
    synch.Commit( clientsPublisher_, model_ );
    synching_ = false;

    // $$$$ AGE 2007-04-24: Le commit va Update le Model et donc supprimer l'entité en question
    // $$$$ AGE 2007-04-24: c'est crado. Séparer l'envoi au client (dans le CommitDestruction() )
    // $$$$ AGE 2007-04-24: et la destruction (depuis la liste, séparément).
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::IsSynching
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
bool SimulationDispatcher::IsSynching() const
{
    return synching_;
}
