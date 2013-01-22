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
#include "clients_kernel/Entity_ABC.h"
#include "EntityPublisher_ABC.h"
#include <boost/bind.hpp>

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
void FullUpdateVisitor::Visit( const kernel::EntityBase_ABC& entity )
{
    entity.GetInterfaces().Apply( &EntityPublisher_ABC::SendFullUpdate, *publisher_ );
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
void CreationVisitor::Visit( const kernel::EntityBase_ABC& entity )
{
    if( entities_.insert( &entity ).second )
        entity.GetInterfaces().Apply( &EntityPublisher_ABC::SendCreation, *publisher_ );
}
