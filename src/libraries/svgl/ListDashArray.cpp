// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListDashArray.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListDashArray constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListDashArray::ListDashArray( unsigned list )
    : DashArray( "" )
    , list_( list )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListDashArray destructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListDashArray::~ListDashArray()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListDashArray::Clone
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListDashArray& ListDashArray::Clone() const
{
    return *new ListDashArray( list_ );
}

// -----------------------------------------------------------------------------
// Name: ListDashArray::ReachTop
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void ListDashArray::ReachTop( const RenderingContext_ABC& )
{
    glCallList( list_ );
}
