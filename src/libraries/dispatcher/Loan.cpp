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
Loan::Loan( const Model& model, const ASN1T_BorrowedEquipment& asnMsg )
    : agent_        ( &model.agents_.Get( asnMsg.oid_pion_preteur ) )
    , equipmentType_( asnMsg.type_equipement )
    , quantity_     ( asnMsg.nombre )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, const ASN1T_LentEquipment& asnMsg )
    : agent_        ( &model.agents_.Get( asnMsg.oid_pion_emprunteur ) )
    , equipmentType_( asnMsg.type_equipement )
    , quantity_     ( asnMsg.nombre )
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
void Loan::Send( ASN1T_BorrowedEquipment& asnMsg ) const
{
    asnMsg.type_equipement  = equipmentType_;
    asnMsg.oid_pion_preteur = agent_->GetId();
    asnMsg.nombre           = quantity_;
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( ASN1T_LentEquipment& asnMsg ) const
{
    asnMsg.type_equipement     = equipmentType_;
    asnMsg.oid_pion_emprunteur = agent_->GetId();
    asnMsg.nombre              = quantity_;
}
