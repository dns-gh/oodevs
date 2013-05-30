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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder, const sword::Diffusion& message,
                                                  const tools::Resolver_ABC< Automat_ABC >& automats,
                                                  const tools::Resolver_ABC< Formation_ABC >& formations,
                                                  const tools::Resolver_ABC< kernel::Agent_ABC >& agents )
    : kernel::TacticalLineHierarchies_ABC( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
    , agents_( agents )
    , diffusion_( eFormation )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( Controller& controller, Entity_ABC& holder, const Entity_ABC& superior
                                                , const tools::Resolver_ABC< Automat_ABC >& automats
                                                , const tools::Resolver_ABC< Formation_ABC >& formations
                                                , const tools::Resolver_ABC< kernel::Agent_ABC >& agents )
    : kernel::TacticalLineHierarchies_ABC( holder, 0 )
    , controller_( controller )
    , automats_  ( automats )
    , formations_( formations )
    , agents_( agents )
    , diffusion_( eFormation )
{
    if( dynamic_cast< const Automat_ABC* >( &superior ) != 0 )
        diffusion_ = eAutomat;
    else if( dynamic_cast< const Agent_ABC* >( &superior ) != 0 )
        diffusion_ = eUnit;
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
    if( message.has_unit() )
        diffusion_ = eUnit;
    else if( message.has_automat() )
        diffusion_ = eAutomat;
    else if( message.has_formation() )
        diffusion_ = eFormation;
    if( diffusion_ == eUnit )
        SetSuperior( &agents_.Get( message.unit().id() ) );
    else if( diffusion_ == eAutomat )
        SetSuperior( &automats_.Get( message.automat().id() ) );
    else if( diffusion_ == eFormation )
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
        throw MASA_EXCEPTION( "Invalid superior." );

    if( diffusion_ == eUnit )
        message.mutable_unit()->set_id( GetSuperior()->GetId() );
    else if( diffusion_ == eAutomat )
        message.mutable_automat()->set_id( GetSuperior()->GetId() );
    else if( diffusion_ == eFormation )
        message.mutable_formation()->set_id( GetSuperior()->GetId() );
}
