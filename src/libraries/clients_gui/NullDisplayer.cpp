// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NullDisplayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: NullDisplayer constructor
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
NullDisplayer::NullDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer destructor
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
NullDisplayer::~NullDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::Hide
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void NullDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::SubItem
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& NullDisplayer::SubItem( const QString&  )
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::StartDisplay
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void NullDisplayer::StartDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::DisplayFormatted
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void NullDisplayer::DisplayFormatted( const QString&  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::EndDisplay
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void NullDisplayer::EndDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void NullDisplayer::Clear()
{
    // NOTHING
}
