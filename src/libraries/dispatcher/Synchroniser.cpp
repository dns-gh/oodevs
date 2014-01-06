// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Synchroniser.h"
#include "EntityPublisher_ABC.h"
#include "ReplaySynchronisations.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Synchroniser constructor
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
Synchroniser::Synchroniser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchroniser destructor
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
Synchroniser::~Synchroniser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForCreation
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForCreation( kernel::EntityBase_ABC& entity )
{
    toCreate_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForUpdate
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForUpdate( kernel::EntityBase_ABC& entity )
{
    toUpdate_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForDestruction( kernel::EntityBase_ABC& entity )
{
    toDestroy_.push_back( &entity );
}

namespace
{
    struct Publisher : public ClientPublisher_ABC
    {
        Publisher( ClientPublisher_ABC& forward, Model& model )
            : forward_( &forward )
            , model_  ( &model ) {}
        virtual void Send( const sword::SimToClient& msg )
        {
            forward_->Send( msg );
            model_->Update( msg );
        }
        virtual void Send( const sword::AuthenticationToClient& ) {}
        virtual void Send( const sword::ReplayToClient& ) {}
        virtual void Send( const sword::AarToClient& ) {}
        virtual void Send( const sword::MessengerToClient& ) {}
        virtual void Send( const sword::DispatcherToClient& ) {}
        ClientPublisher_ABC* forward_;
        Model* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::Commit
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::Commit( ClientPublisher_ABC& publisher, Model& model )
{
    for( auto it = toCreate_.begin(); it != toCreate_.end(); ++it )
        (*it)->GetInterfaces().Apply( &EntityPublisher_ABC::SendCreation, publisher );

    for( auto it = toUpdate_.begin(); it != toUpdate_.end(); ++it )
        (*it)->GetInterfaces().Apply( &EntityPublisher_ABC::SendFullUpdate, publisher );

    Publisher p( publisher, model );
    size_t size = toDestroy_.size();
    for( size_t i = size; i > 0; --i )
    {
        // $$$$ AGE 2008-06-20:
//        (*it)->Apply( &EntityPublisher_ABC::SendDestruction, p );
        // $$$$ AGE 2008-06-20: causes reentrances in the destructor...
        // $$$$ AGE 2008-06-20: Change the whole thing (see SimulationDispatcher::EndSynchronisation)
        auto entity = toDestroy_[i - 1];
        entity->Get< ReplaySynchronisations >().MarkForDestructionNow();
        const EntityPublisher_ABC* publisher = entity->Retrieve< EntityPublisher_ABC >();
        if( publisher )
            publisher->SendDestruction( p );
    }
    toDestroy_.clear();
}
