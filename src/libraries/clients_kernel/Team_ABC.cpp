// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: Team_ABC constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::Team_ABC()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Team_ABC destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::~Team_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Select( ActionController& ) const
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Team_ABC::ContextMenu
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::ContextMenu( ActionController&, const QPoint& ) const
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Team_ABC::Activate
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Activate( ActionController& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::operator==
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
bool Team_ABC::operator==( const Team_ABC& rhs ) const
{
    return this == &rhs;
}
