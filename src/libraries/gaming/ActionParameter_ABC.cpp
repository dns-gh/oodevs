// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::ActionParameter_ABC()
    : id_( 0 ) // $$$$ SBO 2007-03-12: 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::~ActionParameter_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetId
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
unsigned long ActionParameter_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString ActionParameter_ABC::GetName() const
{
    return "";
}
