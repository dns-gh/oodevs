// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder
                                                , const ASN1T_TacticalLinesDiffusion& asnMsg
                                                , const kernel::Resolver_ABC< kernel::Automat_ABC >& automats
                                                , const kernel::Resolver_ABC< kernel::Formation_ABC >& formations )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, holder, 0 )
    , automats_  ( automats )
    , formations_( formations )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior
                                                , const kernel::Resolver_ABC< kernel::Automat_ABC >& automats
                                                , const kernel::Resolver_ABC< kernel::Formation_ABC >& formations )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, holder, 0 )
    , automats_  ( automats )
    , formations_( formations )
{
    superior_ = automats_.Find( superior.GetId() );
    superiorIsAutomat_ = superior_;
    if( !superiorIsAutomat_ )
        superior_ = &formations_.Get( superior.GetId() );
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::~TacticalLineHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::GetSuperior
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* TacticalLineHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::Update
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::Update( const ASN1T_TacticalLinesDiffusion& message )
{
    superiorIsAutomat_ = message.t == T_TacticalLinesDiffusion_automate;
    if( superiorIsAutomat_ )
        superior_ = &automats_.Get( message.u.automate );
    else
        superior_ = &formations_.Get( message.u.formation );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::DoUpdate( const ASN1T_MsgLimaUpdate& message )
{
    Update( message.tactical_line.diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::DoUpdate( const ASN1T_MsgLimitUpdate& message )
{
    Update( message.tactical_line.diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::WriteTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::WriteTo( ASN1T_TacticalLinesDiffusion& message ) const
{
    if( !superior_ )
        throw std::runtime_error( __FUNCTION__ );
    message.t = superiorIsAutomat_ ? T_TacticalLinesDiffusion_automate : T_TacticalLinesDiffusion_formation;
    if( superiorIsAutomat_ )
        message.u.automate  = superior_->GetId();
    else
        message.u.formation = superior_->GetId();
}
