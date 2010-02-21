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
#include "Loan.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Borrowings constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Borrowings::Borrowings( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const tools::Resolver_ABC< EquipmentType >& equipmentResolver )
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
void Borrowings::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( ! message.has_equipements_pretes()  )
        return;

    borrowings_.clear();
    borrowings_.reserve( message.equipements_empruntes().elem_size() );
    for( int i = 0; i < message.equipements_empruntes().elem_size(); ++i )
    {
        const MsgsSimToClient::BorrowedEquipments_BorrowedEquipment& pret = message.equipements_empruntes().elem( i );
        borrowings_.push_back( Loan( equipmentResolver_.Get( pret.type_equipement() ),
                                     resolver_.Get( pret.oid_pion_preteur() ),
                                     pret.nombre() ) );
    }
    controller_.Update( *this );
}
