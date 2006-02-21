// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Lend.h"

// -----------------------------------------------------------------------------
// Name: Lend constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Lend::Lend( const EquipmentType& type, const Agent& borrower, unsigned int quantity )
    : type_( & type )
    , borrower_( & borrower )
    , quantity_( quantity ) 
{

}

// -----------------------------------------------------------------------------
// Name: Lend destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Lend::~Lend()
{

}
