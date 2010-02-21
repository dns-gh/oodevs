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
void HierarchicExtension_ABC::DoUpdate( const MsgsSimToClient::MsgUnitCreation& message )
{
    superior_ = &automatResolver_.Get( message.oid_automate() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const MsgsSimToClient::MsgAutomatCreation& message )
{
    if( message.has_oid_parent() && message.oid_parent().has_automate() )
        superior_ = &automatResolver_.Get( message.oid_parent().automate().oid() );
    else if( message.has_oid_parent() && message.oid_parent().has_formation() )
        superior_ = &formationResolver_.Get( message.oid_parent().formation().oid() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const Common::MsgFormationCreation& message )
{
    if( message.has_oid_formation_parente()  )
        superior_ = &formationResolver_.Get( message.oid_formation_parente() );
    else
        superior_ = &teamResolver_.Get( message.oid_camp() );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const Common::MsgUnitChangeSuperior& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const Common::MsgAutomatChangeSuperior& message )
{
    if( message.has_oid_superior() && message.oid_superior().has_automate() )
        UpdateSuperior( automatResolver_.Get( message.oid_superior().automate().oid() ) );
    else if( message.has_oid_superior() && message.oid_superior().has_formation() )
        UpdateSuperior( formationResolver_.Get( message.oid_superior().formation().oid() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::UpdateSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::UpdateSuperior( const kernel::Entity_ABC& superior )
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
