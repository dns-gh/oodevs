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
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgUnitCreation& message )
{
    superior_ = &automatResolver_.Get( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgAutomatCreation& message )
{
    if( message.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate )
        superior_ = &automatResolver_.Get( message.oid_parent.u.automate );
    else if( message.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation )
        superior_ = &formationResolver_.Get( message.oid_parent.u.formation );
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
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgUnitChangeSuperior& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgUnitChangeSuperiorAck& message )
{
    if( !message.error_code )
        UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchicExtension_ABC::DoUpdate
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void HierarchicExtension_ABC::DoUpdate( const ASN1T_MsgAutomatChangeSuperiorAck& message )
{
    if( message.error_code )
        return;
    if( message.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
        UpdateSuperior( automatResolver_.Get( message.oid_superior.u.automate ) );
    else if( message.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
        UpdateSuperior( formationResolver_.Get( message.oid_superior.u.formation ) );
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
