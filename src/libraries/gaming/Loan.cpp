// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Loan.h"

// -----------------------------------------------------------------------------
// Name: Loan constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Loan::Loan( const EquipmentType& type, const Agent_ABC& agent, unsigned int quantity )
    : type_( & type )
    , agent_( & agent )
    , quantity_( quantity ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loan destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Loan::~Loan()
{
    // NOTHING
}
