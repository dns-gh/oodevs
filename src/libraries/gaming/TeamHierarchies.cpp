// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( Controller& controller, kernel::Team_ABC& holder )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, 0 )
    , controller_( controller )
    , holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    controller_.Delete( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::DoUpdate
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
void TeamHierarchies::DoUpdate( const sword::KnowledgeGroupUpdate& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool TeamHierarchies::CanCommunicate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::CanReceive
// Created: LDC 2013-09-27
// -----------------------------------------------------------------------------
bool TeamHierarchies::CanReceive() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool TeamHierarchies::IsJammed() const
{
    return false;
}

