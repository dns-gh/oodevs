// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListPaint.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListPaint constructor
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListPaint::ListPaint( unsigned list )
    : list_( list )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListPaint destructor
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListPaint::~ListPaint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListPaint::Setup
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
bool ListPaint::Setup( References_ABC& , float ) const
{
    glCallList( list_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ListPaint::Clone
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListPaint& ListPaint::Clone() const
{
    return *new ListPaint( list_ );
}
