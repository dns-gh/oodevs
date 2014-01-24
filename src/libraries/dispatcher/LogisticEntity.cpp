// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticEntity.h"
#include "LogisticEntityOwner_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver_ABC.h>
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticEntity constructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticEntity::LogisticEntity( const LogisticEntityOwner_ABC& owner,
                                const tools::Resolver_ABC< Formation_ABC >& formations,
                                const tools::Resolver_ABC< Automat_ABC >& automats )
    : owner_( owner )
    , logisticHierarchy_( *this, formations, automats )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity destructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticEntity::~LogisticEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::GetLogisticHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
LogisticHierarchy& LogisticEntity::GetLogisticHierarchy()
{
    return logisticHierarchy_;
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticEntity::Send( sword::ParentEntity& msg ) const
{
    owner_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticEntity::Send( sword::AutomatCreation& msg ) const
{
    msg.set_logistic_level( sword::logistic_base );
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticEntity::Send( sword::FormationCreation& msg ) const
{
    msg.set_logistic_level( sword::logistic_base );
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::SendFullUpdate
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticEntity::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    logisticHierarchy_.SendFullUpdate( publisher );
}
