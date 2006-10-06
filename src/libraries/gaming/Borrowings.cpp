// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Borrowings.h"
#include "clients_kernel/Controller.h"
#include "Loan.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Borrowings constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Borrowings::Borrowings( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< EquipmentType >& equipmentResolver )
    : controller_( controller )
    , resolver_( resolver )
    , equipmentResolver_( equipmentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Borrowings destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Borrowings::~Borrowings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Borrowings::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Borrowings::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( ! message.m.equipements_pretesPresent )
        return;

    borrowings_.clear();
    borrowings_.reserve( message.equipements_empruntes.n );
    for( unsigned int i = 0; i < message.equipements_empruntes.n; ++i )
    {
        const ASN1T_EquipementEmprunte& pret = message.equipements_empruntes.elem[i];
        borrowings_.push_back( Loan( equipmentResolver_.Get( pret.type_equipement ),
                                     resolver_.Get( pret.oid_pion_preteur ),
                                     pret.nombre ) );
    }
    controller_.Update( *this );
}
