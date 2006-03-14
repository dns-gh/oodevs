// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "IndirectFire.h"
#include "Agent.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: IndirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::IndirectFire( const ASN1T_MsgStartPionFire& message, const Resolver_ABC< Agent >& resolver, const CoordinateConverter& converter )
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
