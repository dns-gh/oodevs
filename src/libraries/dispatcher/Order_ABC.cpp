// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Order_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Order_ABC constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::Order_ABC( unsigned long id )
    : id_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Order_ABC destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::~Order_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::GetId
// Created: SBO 2009-10-05
// -----------------------------------------------------------------------------
unsigned long Order_ABC::GetId() const
{
    return id_;
}
