// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListLength.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListLength constructor
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListLength::ListLength( unsigned list )
     : Length( "" )
     , list_( list )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListLength destructor
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListLength::~ListLength()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ListLength::Clone
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
ListLength& ListLength::Clone() const
{
    return *new ListLength( list_ );
}

// -----------------------------------------------------------------------------
// Name: ListLength::ReachTop
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
void ListLength::ReachTop( const RenderingContext_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListLength::SetupLineWidth
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
void ListLength::SetupLineWidth() const
{
    glCallList( list_ );
}
