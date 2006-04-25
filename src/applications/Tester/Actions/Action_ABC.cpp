// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Action_ABC.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Action_ABC constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Action_ABC::Action_ABC()
    : strName_        ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Action_ABC::~Action_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::CheckParameters
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
bool Action_ABC::CheckParameters() const
{
    return true;
}
