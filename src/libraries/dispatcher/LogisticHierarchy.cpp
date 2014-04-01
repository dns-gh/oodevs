// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticHierarchy.h"
#include "Automat_ABC.h"
#include "Formation_ABC.h"
#include "Model_ABC.h"
#include "LogisticEntity.h"
#include "LogisticHierarchyOwner_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver_ABC.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy constructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticHierarchy::LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< Automat_ABC >& automats )
    : owner_     ( owner )
    , formations_( formations )
    , automats_  ( automats )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy destructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticHierarchy::~LogisticHierarchy()
{
    superiorLinks_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::DoUpdate
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
void LogisticHierarchy::DoUpdate( const sword::ChangeLogisticLinks& msg )
{
    superiorLinks_.clear();
    const auto& superior = msg.superior();
    for( auto it = superior.begin(); it != superior.end(); ++it )
    {
        const LogisticEntity_ABC* superior = FindLogisticEntity( *it );
        assert( superior );
        superiorLinks_.push_back( boost::shared_ptr< LogisticLink >( new LogisticLink( owner_, *superior ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::DoUpdate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void LogisticHierarchy::DoUpdate( const sword::LogSupplyQuotas& msg )
{
    LogisticEntity_ABC* pSupplier = FindLogisticEntity( msg.supplier() );
    assert( pSupplier );
    LogisticLink* link = FindLogisticLink( *pSupplier );
    if( link )
        link->Update( msg.quotas() );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SendFullUpdate
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticHierarchy::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    // Links
    client::ChangeLogisticLinks msg;
    owner_.Send( *msg().mutable_requester() );
    for( auto it = superiorLinks_.begin(); it != superiorLinks_.end(); ++it )
        (*it)->GetSuperior().Send( *msg().add_superior() );
    msg.Send( publisher );

    // Links attributes
    for( auto it = superiorLinks_.begin(); it != superiorLinks_.end(); ++it )
        (*it)->SendFullUpdate( publisher );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::FindLogisticEntity
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticEntity_ABC* LogisticHierarchy::FindLogisticEntity( const sword::ParentEntity& msg ) const
{
    if( msg.has_automat() )
        return automats_.Find( msg.automat().id() )->GetLogisticEntity();

    if( msg.has_formation() )
        return formations_.Find( msg.formation().id() )->GetLogisticEntity();

    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::FindLogisticLink
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticLink* LogisticHierarchy::FindLogisticLink( const LogisticEntity_ABC& superior ) const
{
    for( auto it = superiorLinks_.begin(); it != superiorLinks_.end(); ++it )
    {
        if( &(*it)->GetSuperior() == &superior )
            return it->get();
    }
    return 0;
}
