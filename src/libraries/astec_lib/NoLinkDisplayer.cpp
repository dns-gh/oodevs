// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "NoLinkDisplayer.h"
#include "Agent_ABC.h"
#include "Population.h"
#include "Object.h"

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer constructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
NoLinkDisplayer::NoLinkDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer destructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
NoLinkDisplayer::~NoLinkDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Agent_ABC& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Population& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Object& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}
