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
    if( ! message.m.prets_equipementPresent )
        return;

    lends_.clear();
    lends_.reserve( message.prets_equipement.n );
    for( unsigned int i = 0; i < message.prets_equipement.n; ++i )
    {
        const ASN1T_PretEquipement& pret = message.prets_equipement.elem[i];
        lends_.push_back( Lend( equipmentResolver_.Get( pret.type_equipement ),
                                resolver_.Get( pret.oid_pion_emprunteur ),
                                pret.nombre ) );
    }
    controller_.Update( *this );
}
