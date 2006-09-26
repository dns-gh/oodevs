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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, ASN1T_EquipementEmprunte& asnMsg )
    : pAgent_        ( &model.GetAgents().Get( asnMsg.oid_pion_preteur ) )
    , nEquipmentType_( asnMsg.type_equipement )
    , nQuantity_     ( asnMsg.nombre          )
{
}

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Loan::Loan( const Model& model, ASN1T_EquipementPrete& asnMsg )
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
    
    