// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLineHierarchies.h"
#include "TacticalLines.h"

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::TacticalLineHierarchies_ABC( holder, superior )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
TacticalLineHierarchies::~TacticalLineHierarchies()
{
    if( superior_ )
        if( TacticalLines* lines = superior_->Retrieve< TacticalLines >() )
            lines->Remove( GetEntity().GetId() );
}
