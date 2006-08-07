// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "FragOrder.h"

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder::FragOrder( const std::string& name, unsigned int id )
    : name_( name )
    , id_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder::~FragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetId
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long FragOrder::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
std::string FragOrder::GetName() const
{
    return name_;
}
