// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DirectFire.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"

// -----------------------------------------------------------------------------
// Name: DirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::DirectFire( const ASN1T_MsgStartPionFire& message, const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< Population_ABC >& populationResolver )
    : Fire_ABC( agentResolver.Get( message.tireur ) )
    , id_( message.oid_tir )
{
    if( message.cible.t == T_MsgStartPionFire_cible_pion )
        target_ = & agentResolver.Get( message.cible.u.pion );
    else if( message.cible.t == T_MsgStartPionFire_cible_population )
        target_ = & populationResolver.Get( message.cible.u.population );
    else
        throw std::runtime_error( "DirectFire on position..." );

    if( message.m.munitionPresent )
        ; // $$$$ AGE 2006-03-10: 
}

// -----------------------------------------------------------------------------
// Name: DirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::~DirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DirectFire::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    const Positions* p = target_->Retrieve< Positions >();
    if( p && ! viewport.Intersect( geometry::Rectangle2f( where, p->GetPosition() ) ).IsEmpty() )
        tools.DrawArrow( where, p->GetPosition() );
}
