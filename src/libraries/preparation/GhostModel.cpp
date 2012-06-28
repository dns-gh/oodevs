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
#include "Model.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: GhostModel constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostModel::GhostModel( kernel::Controllers& controllers, GhostFactory_ABC& ghostFactory )
    : controllers_         ( controllers )
    , ghostFactory_        ( ghostFactory )
    , hasConvertNeedSaving_( false )
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
// Name: GhostModel::Create from creation panel
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostModel::Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position )
{
    kernel::Ghost_ABC* ghost = ghostFactory_.Create( parent, prototype, position );
    Register( ghost->GetId(), *ghost );
    return ghost;
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Create from xml
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
void GhostModel::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    kernel::Ghost_ABC* ghost = ghostFactory_.Create( parent, xis );
    Register( ghost->GetId(), *ghost );
    if( ghost->GetGhostType() == eGhostType_Automat )
    {
        ghost->ReadGhostAttributes( xis );
        xis >> xml::list( "phantom", *this, &GhostModel::Create, *ghost );
    }
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Create from unknown type conversion
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostModel::Create( xml::xistream& xis, kernel::Entity_ABC& parent, E_GhostType ghostType )
{
    kernel::Ghost_ABC* ghost = ghostFactory_.Create( parent, xis, ghostType );
    Register( ghost->GetId(), *ghost );
    if( ghostType == eGhostType_Automat )
    {
        E_GhostType childGhostType = eGhostType_Agent;
        xis >> xml::list( "unit", *this, &GhostModel::Create, *ghost, childGhostType );
        childGhostType = eGhostType_Automat;
        xis >> xml::list( "automat", *this, &GhostModel::Create, *ghost, childGhostType )
            >> xml::list( "phantom", *this, &GhostModel::Create, *ghost );
    }
    if( !hasConvertNeedSaving_ )
        hasConvertNeedSaving_ = true;
}

// -----------------------------------------------------------------------------
// Name: GhostModel::Finalize
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void GhostModel::Finalize( const StaticModel& staticModel )
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Finalize( staticModel );
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

// -----------------------------------------------------------------------------
// Name: GhostModel::NeedSaving
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
bool GhostModel::NeedSaving() const
{
    return hasConvertNeedSaving_;
}
