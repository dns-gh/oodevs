// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Loan.h"
#include "Agent_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

namespace
{
    int counter = 0;
}
// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model_ABC&, const sword::BorrowedEquipments_BorrowedEquipment& message )
    : idAgent_      ( message.owner().id() )
    , equipmentType_( message.type().id() )
    , quantity_     ( message.quantity() )
    , nCounter_( ++counter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model_ABC&, const sword::LentEquipments_LentEquipment& message )
    : idAgent_      ( message.borrower().id() )
    , equipmentType_( message.type().id() )
    , quantity_     ( message.quantity() )
    , nCounter_( ++counter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::~Loan()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( sword::BorrowedEquipments_BorrowedEquipment& message ) const
{
    message.mutable_type()->set_id( equipmentType_ );
    message.mutable_owner()->set_id( idAgent_ );
    message.set_quantity( quantity_ );
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( sword::LentEquipments_LentEquipment& message ) const
{
    message.mutable_type()->set_id( equipmentType_ );
    message.mutable_borrower()->set_id( idAgent_ );
    message.set_quantity( quantity_ );
}
