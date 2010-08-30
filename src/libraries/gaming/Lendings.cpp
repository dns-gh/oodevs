// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Lendings.h"
#include "Loan.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Lendings constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lendings::Lendings( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const tools::Resolver_ABC< EquipmentType >& equipmentResolver )
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
void Lendings::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( ! message.has_equipements_pretes()  )
        return;

    lendings_.clear();
    lendings_.reserve( message.equipements_pretes().elem_size() );
    for( int i = 0; i < message.equipements_pretes().elem_size(); ++i )
    {
        const MsgsSimToClient::LentEquipments_LentEquipment& pret = message.equipements_pretes().elem( i );
        lendings_.push_back( Loan( equipmentResolver_.Get( pret.type().id() ),
                                   resolver_.Get( pret.borrower().id() ),
                                   pret.nombre() ) );
    }
    controller_.Update( *this );
}
