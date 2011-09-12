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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Model.h"
#include "Synchroniser.h"
#include "ReplaySynchronisations.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

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
// Name: SimulationDispatcher::IsDestruction
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool SimulationDispatcher::IsDestruction( const sword::SimToClient& wrapper ) const
{
    if( wrapper.message().has_unit_knowledge_destruction() ||
        wrapper.message().has_object_destruction() ||
        wrapper.message().has_object_knowledge_destruction() ||
        wrapper.message().has_log_medical_handling_destruction() ||
        wrapper.message().has_log_maintenance_handling_destruction() ||
        wrapper.message().has_log_supply_handling_destruction() ||
        wrapper.message().has_crowd_concentration_destruction() ||
        wrapper.message().has_crowd_flow_destruction() ||
        wrapper.message().has_crowd_knowledge_destruction() ||
        wrapper.message().has_crowd_concentration_knowledge_destruction() ||
        wrapper.message().has_crowd_flow_knowledge_destruction() ||
        wrapper.message().has_stop_fire_effect() ||
        wrapper.message().has_stop_unit_fire() ||
        wrapper.message().has_stop_crowd_fire() ||
        wrapper.message().has_invalidate_report() )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::Receive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SimulationDispatcher::Receive( const sword::SimToClient& asnMsg )
{
    if( !synching_ || IsDestruction( asnMsg )
        || ( asnMsg.message().has_control_begin_tick() && asnMsg.message().control_begin_tick().current_tick() == 0 )
        || ( asnMsg.message().has_control_end_tick() && asnMsg.message().control_end_tick().current_tick() == 0 ) )
        clientsPublisher_.Send( asnMsg );
}

namespace
{
    class StartSynchVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        virtual void Visit( const kernel::Entity_ABC& entity )
        {
            const_cast< kernel::Entity_ABC& >( entity ).Get< ReplaySynchronisations >().StartSynchronisation( false );
        }
    };

    class EndSynchVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        EndSynchVisitor( Synchroniser& model )
            : model_( &model ) {}
        virtual void Visit( const kernel::Entity_ABC& entity )
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
