// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Lends.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Lends constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lends::Lends( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}

// -----------------------------------------------------------------------------
// Name: Lends destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lends::~Lends()
{

}

// -----------------------------------------------------------------------------
// Name: Lends::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Lends::DoUpdate( const ASN1T_MsgUnitDotations& message )
{
    if( ! message.m.prets_equipementPresent )
        return;

    lends_.resize( message.prets_equipement.n );
    for( unsigned int i = 0; i < message.prets_equipement.n; ++i )
    {
        lends_[i].borrower_   = resolver_.Find( message.prets_equipement.elem[i].oid_pion_emprunteur );
        lends_[i].nEquipment_ = message.prets_equipement.elem[i].type_equipement; // $$$$ AGE 2006-02-13: resolve
        lends_[i].nQuantity_  = message.prets_equipement.elem[i].nombre;
    }

    controller_.Update( *this );
}
