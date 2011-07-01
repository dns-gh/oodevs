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
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder, const sword::Diffusion& message,
                                                  const tools::Resolver_ABC< Automat_ABC >& automats,
                                                  const tools::Resolver_ABC< Formation_ABC >& formations )
    : SimpleHierarchies< TacticalHierarchies >( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder, const Entity_ABC& superior
                                                , const tools::Resolver_ABC< Automat_ABC >& automats
                                                , const tools::Resolver_ABC< Formation_ABC >& formations )
    : SimpleHierarchies< TacticalHierarchies >( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
{
    superiorIsAutomat_ = dynamic_cast< const Automat_ABC* >( &superior ) != 0;
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
void TacticalLineHierarchies::Update( const sword::Diffusion& message )
{
    superiorIsAutomat_ = message.has_automat();
    if( superiorIsAutomat_ )
        SetSuperior( &automats_.Get( message.automat().id() ) );
    else
        SetSuperior( &formations_.Get( message.formation().id() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::DoUpdate( const sword::PhaseLineUpdate& message )
{
    Update( message.tactical_line().diffusion() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::DoUpdate( const sword::LimitUpdate& message )
{
    Update( message.tactical_line().diffusion() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::WriteTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLineHierarchies::WriteTo( sword::Diffusion& message ) const
{
    if( !GetSuperior() )
        throw std::runtime_error( __FUNCTION__ );

    if( superiorIsAutomat_ )
        message.mutable_automat()->set_id( GetSuperior()->GetId() );
    else
        message.mutable_formation()->set_id( GetSuperior()->GetId() );
}
