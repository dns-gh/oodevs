// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Mission.h"
#include "FragOrder.h"

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission::Mission( const std::string& name, unsigned int id, bool automat )
    : name_( name )
    , id_( id )
    , automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Mission::AddAvailableOrder
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Mission::AddAvailableOrder( FragOrder& order )
{
    Register( order.id_, order );
}
