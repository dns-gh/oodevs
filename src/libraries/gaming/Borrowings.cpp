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
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Borrowings destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Borrowings::~Borrowings()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Borrowings::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Borrowings::DoUpdate( const sword::UnitAttributes& message )
{
    if( ! message.has_borrowed_equipments()  )
        return;

    borrowings_.clear();
    borrowings_.reserve( message.borrowed_equipments().elem_size() );
    for( int i = 0; i < message.borrowed_equipments().elem_size(); ++i )
    {
        const sword::BorrowedEquipments_BorrowedEquipment& pret = message.borrowed_equipments().elem( i );
        borrowings_.push_back( Loan( equipmentResolver_.Get( pret.type().id() ),
                                     resolver_.Get( pret.owner().id() ),
                                     pret.quantity() ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Borrowings::NotifyDeleted
// Created: ABR 2014-02-26
// -----------------------------------------------------------------------------
void Borrowings::NotifyDeleted( const kernel::Agent_ABC& agent )
{
    borrowings_.erase( std::remove_if( borrowings_.begin(),
                                       borrowings_.end(),
                                       [&]( const Loan& loan ) { return loan.agent_ == &agent; } ),
                       borrowings_.end() );
}
