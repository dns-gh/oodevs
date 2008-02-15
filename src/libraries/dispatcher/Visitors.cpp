// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Visitors.h"
#include "Entity_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::FullUpdateVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
FullUpdateVisitor::FullUpdateVisitor( ClientPublisher_ABC& publisher )
    : publisher_( &publisher )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Entity_ABC& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::CreationVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
CreationVisitor::CreationVisitor( ClientPublisher_ABC& publisher )
    : publisher_( &publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Entity_ABC& entity )
{
    entity.SendCreation( *publisher_ );
}
