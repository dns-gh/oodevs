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

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
HierarchicExtension_ABC::HierarchicExtension_ABC( const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver
                                                , const kernel::Resolver_ABC< kernel::Formation_ABC >& formationResolver
                                                , const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver )
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
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgPionCreation& message )
{
    superior_ = &automatResolver_.Get( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    superior_ = &formationResolver_.Get( message.oid_formation );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgFormationCreation& message )
{
    if( message.m.oid_formation_parentePresent )
        superior_ = &formationResolver_.Get( message.oid_formation_parente );
    else
        superior_ = &teamResolver_.Get( message.oid_camp );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgPionChangeSuperior& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgPionChangeSuperiorAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error )
        UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
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
