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
#include "Model.h"
#include "Agent.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, const MsgsSimToClient::BorrowedEquipments_BorrowedEquipment& message )
    : agent_        ( &model.agents_.Get( message.oid_pion_preteur() ) )
    , equipmentType_( message.type_equipement() )
    , quantity_     ( message.nombre() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, const MsgsSimToClient::LentEquipments_LentEquipment& message )
    : agent_        ( &model.agents_.Get( message.oid_pion_emprunteur() ) )
    , equipmentType_( message.type_equipement() )
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
void Loan::Send( MsgsSimToClient::BorrowedEquipments_BorrowedEquipment& message ) const
{
    message.set_type_equipement( equipmentType_ );
    message.set_oid_pion_preteur( agent_->GetId() );
    message.set_nombre( quantity_ );
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( MsgsSimToClient::LentEquipments_LentEquipment& message ) const
{
    message.set_type_equipement( equipmentType_ );
    message.set_oid_pion_emprunteur( agent_->GetId() );
    message.set_nombre( quantity_ );
}
