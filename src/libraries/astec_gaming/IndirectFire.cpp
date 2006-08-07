// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "IndirectFire.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_gui/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: IndirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::IndirectFire( const ASN1T_MsgStartPionFire& message, const Resolver_ABC< Agent_ABC >& resolver, const CoordinateConverter_ABC& converter )
    : Fire_ABC( resolver.Get( message.tireur ) )
    , id_( message.oid_tir )
    , target_( converter.ConvertToXY( *message.cible.u.position ) )
{
    if( message.cible.t != T_MsgStartPionFire_cible_position )
        throw std::runtime_error( "Indirect fire on an agent..." );
}

// -----------------------------------------------------------------------------
// Name: IndirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::~IndirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void IndirectFire::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.Intersect( geometry::Rectangle2f( where, target_ ) ).IsEmpty() )
        tools.DrawCurvedArrow( where, target_ );
}
