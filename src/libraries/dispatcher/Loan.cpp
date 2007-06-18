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
Loan::Loan( const Model& model, ASN1T_BorrowedEquipment& asnMsg )
    : pAgent_        ( &model.GetAgents().Get( asnMsg.oid_pion_preteur ) )
    , nEquipmentType_( asnMsg.type_equipement )
    , nQuantity_     ( asnMsg.nombre          )
{
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, ASN1T_LentEquipment& asnMsg )
    : pAgent_        ( &model.GetAgents().Get( asnMsg.oid_pion_emprunteur ) )
    , nEquipmentType_( asnMsg.type_equipement )
    , nQuantity_     ( asnMsg.nombre          )
{
}

// -----------------------------------------------------------------------------
// Name: Loan destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::~Loan()
{

}
    
// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( ASN1T_BorrowedEquipment& asnMsg ) const
{
    asnMsg.type_equipement  = nEquipmentType_;
    asnMsg.oid_pion_preteur = pAgent_->GetID();
    asnMsg.nombre           = nQuantity_;
}

// -----------------------------------------------------------------------------
// Name: Loan::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Loan::Send( ASN1T_LentEquipment& asnMsg ) const
{
    asnMsg.type_equipement     = nEquipmentType_;
    asnMsg.oid_pion_emprunteur = pAgent_->GetID();
    asnMsg.nombre              = nQuantity_;
}
