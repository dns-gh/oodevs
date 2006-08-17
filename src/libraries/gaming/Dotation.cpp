// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Dotation.h"

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Dotation::Dotation()
    : type_( 0 )
    , quantity_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::Dotation( const DotationType& type, unsigned int quantity )
    : type_( & type )
    , quantity_( quantity )
{

}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{

}
