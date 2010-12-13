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
#include "Model_ABC.h"
#include "Agent_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model_ABC& model, const sword::BorrowedEquipments_BorrowedEquipment& message )
    : agent_        ( model.Agents().Get( message.owner().id() ) )
    , equipmentType_( message.type().id() )
    , quantity_     ( message.nombre() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model_ABC& model, const sword::LentEquipments_LentEquipment& message )
    : agent_        ( model.Agents().Get( message.borrower().id() ) )
    , equipmentType_( message.type().id() )
    , quantity_     ( message.nombre() )
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
    message.mutable_owner()->set_id( agent_.GetId() );
    message.set_nombre( quantity_ );
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( sword::LentEquipments_LentEquipment& message ) const
{
    message.mutable_type()->set_id( equipmentType_ );
    message.mutable_borrower()->set_id( agent_.GetId() );
    message.set_nombre( quantity_ );
}
