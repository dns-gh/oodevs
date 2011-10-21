// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostModel.h"
#include "GhostFactory_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: GhostModel constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostModel::GhostModel( kernel::Controllers& controllers, GhostFactory_ABC& ghostFactory )
    : controllers_ ( controllers )
    , ghostFactory_( ghostFactory )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostModel destructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostModel::~GhostModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Create
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostModel::Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position )
{
    kernel::Ghost_ABC* ghost = ghostFactory_.Create( parent, prototype, position );
    Register( ghost->GetId(), *ghost );
    return ghost;
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Create
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostModel::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    kernel::Ghost_ABC* ghost = ghostFactory_.Create( parent, xis );
    Register( ghost->GetId(), *ghost );
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Purge
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: GhostModel::NotifyDeleted
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostModel::NotifyDeleted( const kernel::Ghost_ABC& ghost )
{
    Remove( ghost.GetId() );
}
