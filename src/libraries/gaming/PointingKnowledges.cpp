// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PointingKnowledges.h"

// -----------------------------------------------------------------------------
// Name: PointingKnowledges constructor
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
PointingKnowledges::PointingKnowledges()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges destructor
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
PointingKnowledges::~PointingKnowledges()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::Add
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
void PointingKnowledges::Add( const kernel::Knowledge_ABC& knowledge )
{
    knowledges_.insert( &knowledge );
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::Remove
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
void PointingKnowledges::Remove( const kernel::Knowledge_ABC& knowledge )
{
    knowledges_.erase( &knowledge );
}
