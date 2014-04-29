// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "HierarchicExtension_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
HierarchicExtension_ABC::HierarchicExtension_ABC( const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver
                                                , const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver
                                                , const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : automatResolver_( automatResolver )
    , formationResolver_( formationResolver )
    , teamResolver_( teamResolver )
    , superior_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC destructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
HierarchicExtension_ABC::~HierarchicExtension_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::UnitCreation& message )
{
    superior_ = &automatResolver_.Get( message.automat().id() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::AutomatCreation& message )
{
    if( message.has_parent() && message.parent().has_automat() )
        superior_ = &automatResolver_.Get( message.parent().automat().id() );
    else if( message.has_parent() && message.parent().has_formation() )
        superior_ = &formationResolver_.Get( message.parent().formation().id() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::FormationCreation& message )
{
    if( message.has_parent()  )
        superior_ = &formationResolver_.Get( message.parent().id() );
    else
        superior_ = &teamResolver_.Get( message.party().id() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::UnitChangeSuperior& message )
{
    UpdateSuperior( automatResolver_.Get( message.parent().id() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::AutomatChangeSuperior& message )
{
    if( message.has_superior() && message.superior().has_automat() )
        UpdateSuperior( automatResolver_.Get( message.superior().automat().id() ) );
    else if( message.has_superior() && message.superior().has_formation() )
        UpdateSuperior( formationResolver_.Get( message.superior().formation().id() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::UpdateSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::UpdateSuperior( kernel::Entity_ABC& superior )
{
    SetSuperior( superior );
    superior_ = &superior;
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::GetSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* HierarchicExtension_ABC::GetSuperior() const
{
    return superior_;
}

kernel::Entity_ABC* HierarchicExtension_ABC::GetSuperior()
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: AHC 2013-01-14
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const sword::FormationChangeSuperior& message )
{
    if( message.superior().has_formation() )
        UpdateSuperior( formationResolver_.Get( message.superior().formation().id() ) );
    else
        UpdateSuperior( teamResolver_.Get( message.superior().party().id() ) );
}

