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
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder
                                                , const ASN1T_TacticalLinesDiffusion& asnMsg
                                                , const Resolver_ABC< Automat_ABC >& automats
                                                , const Resolver_ABC< Formation_ABC >& formations )
    : SimpleHierarchies< TacticalHierarchies >( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder, const Entity_ABC& superior
                                                , const Resolver_ABC< Automat_ABC >& automats
                                                , const Resolver_ABC< Formation_ABC >& formations )
    : SimpleHierarchies< TacticalHierarchies >( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
{
    superiorIsAutomat_ = dynamic_cast< const Automat_ABC* >( &superior );
    SetSuperior( &superior );
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
// Name: TacticalLineHierarchies::Update
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::Update( const ASN1T_TacticalLinesDiffusion& message )
{
    superiorIsAutomat_ = message.t == T_TacticalLinesDiffusion_automate;
    if( superiorIsAutomat_ )
        SetSuperior( &automats_.Get( message.u.automate ) );
    else
        SetSuperior( &formations_.Get( message.u.formation ) );
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
    if( !GetSuperior() )
        throw std::runtime_error( __FUNCTION__ );
    message.t = superiorIsAutomat_ ? T_TacticalLinesDiffusion_automate : T_TacticalLinesDiffusion_formation;
    if( superiorIsAutomat_ )
        message.u.automate  = GetSuperior()->GetId();
    else
        message.u.formation = GetSuperior()->GetId();
}
