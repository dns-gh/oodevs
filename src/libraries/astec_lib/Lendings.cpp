// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Lendings.h"
#include "Controller.h"
#include "Loan.h"

// -----------------------------------------------------------------------------
// Name: Lendings constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lendings::Lendings( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< EquipmentType >& equipmentResolver )
    : controller_( controller )
    , resolver_( resolver )
    , equipmentResolver_( equipmentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lendings destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lendings::~Lendings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lendings::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Lendings::DoUpdate( const ASN1T_MsgUnitDotations& message )
{
    if( ! message.m.equipements_pretesPresent )
        return;

    lendings_.clear();
    lendings_.reserve( message.equipements_pretes.n );
    for( unsigned int i = 0; i < message.equipements_pretes.n; ++i )
    {
        const ASN1T_EquipementPrete& pret = message.equipements_pretes.elem[i];
        lendings_.push_back( Loan( equipmentResolver_.Get( pret.type_equipement ),
                                   resolver_.Get( pret.oid_pion_emprunteur ),
                                   pret.nombre ) );
    }
    controller_.Update( *this );
}
