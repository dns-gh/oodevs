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
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Lendings destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Lendings::~Lendings()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Lendings::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Lendings::DoUpdate( const sword::UnitAttributes& message )
{
    if( ! message.has_lent_equipments()  )
        return;

    lendings_.clear();
    lendings_.reserve( message.lent_equipments().elem_size() );
    for( int i = 0; i < message.lent_equipments().elem_size(); ++i )
    {
        const sword::LentEquipments_LentEquipment& pret = message.lent_equipments().elem( i );
        lendings_.push_back( Loan( equipmentResolver_.Get( pret.type().id() ),
                                   resolver_.Get( pret.borrower().id() ),
                                   pret.quantity() ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Lendings::NotifyDeleted
// Created: ABR 2014-02-26
// -----------------------------------------------------------------------------
void Lendings::NotifyDeleted( const kernel::Agent_ABC& agent )
{
    lendings_.erase( std::remove_if( lendings_.begin(),
                                     lendings_.end(),
                                     [&]( const Loan& loan ) { return loan.agent_ == &agent; } ),
                     lendings_.end() );
}
