// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ReplaySynchronizer.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "ReplaySynchronisations.h"
#include "Synchroniser.h"
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplaySynchronizer::ReplaySynchronizer
// Created: SLI 2014-10-13
// -----------------------------------------------------------------------------
ReplaySynchronizer::ReplaySynchronizer( ClientPublisher_ABC& publisher, Model& model )
    : publisher_( publisher )
    , model_    ( model )
    , synching_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronizer::~ReplaySynchronizer
// Created: SLI 2014-10-13
// -----------------------------------------------------------------------------
ReplaySynchronizer::~ReplaySynchronizer()
{
    // NOTHING
}

namespace
{
    class StartSynchVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        virtual void Visit( const kernel::EntityBase_ABC& entity )
        {
            const_cast< kernel::EntityBase_ABC& >( entity ).Get< ReplaySynchronisations >().StartSynchronisation( false );
        }
    };

    class EndSynchVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        EndSynchVisitor( Synchroniser& model )
            : model_( &model ) {}
        virtual void Visit( const kernel::EntityBase_ABC& entity )
        {
            const_cast< kernel::EntityBase_ABC& >( entity ).Get< ReplaySynchronisations >().EndSynchronisation( *model_ );
        }
    private:
        Synchroniser* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronizer::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void ReplaySynchronizer::StartSynchronisation()
{
    synching_ = true;
    StartSynchVisitor visitor;
    model_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronizer::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void ReplaySynchronizer::EndSynchronisation()
{
    Synchroniser synch;
    EndSynchVisitor visitor( synch );
    model_.Accept( visitor );
    synch.Commit( publisher_, model_ );
    synching_ = false;

    // $$$$ AGE 2007-04-24: Le commit va Update le Model et donc supprimer l'entité en question
    // $$$$ AGE 2007-04-24: c'est crado. Séparer l'envoi au client (dans le CommitDestruction() )
    // $$$$ AGE 2007-04-24: et la destruction (depuis la liste, séparément).
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronizer::IsSynching
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
bool ReplaySynchronizer::IsSynching() const
{
    return synching_;
}
