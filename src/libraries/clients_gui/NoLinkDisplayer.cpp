// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NoLinkDisplayer.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;
using namespace gui;

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
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Population_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Object_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: NoLinkDisplayer::Call
// Created: AGE 2008-01-21
// -----------------------------------------------------------------------------
void NoLinkDisplayer::Call( const Formation_ABC& value )
{
    AddToDisplay( value.GetName() );
}
