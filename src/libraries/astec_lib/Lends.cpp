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
#include "Lend.h"

// -----------------------------------------------------------------------------
// Name: Lends constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lends::Lends( Controller& controller, const Resolver_ABC< Agent >& resolver, const Resolver_ABC< EquipmentType >& equipmentResolver )
    : controller_( controller )
    , resolver_( resolver )
    , equipmentResolver_( equipmentResolver )
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
    if( ! message.m.equipements_pretesPresent )
        return;

    lends_.clear();
    lends_.reserve( message.equipements_pretes.n );
    for( unsigned int i = 0; i < message.equipements_pretes.n; ++i )
    {
        const ASN1T_EquipementPrete& pret = message.equipements_pretes.elem[i];
        lends_.push_back( Lend( equipmentResolver_.Get( pret.type_equipement ),
                                resolver_.Get( pret.oid_pion_emprunteur ),
                                pret.nombre ) );
    }
    controller_.Update( *this );
}
