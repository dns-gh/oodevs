// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TeamSelectionObserver.h"
#include "Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
TeamSelectionObserver::TeamSelectionObserver()
    : selected_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver destructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
TeamSelectionObserver::~TeamSelectionObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::BeforeSelection
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::BeforeSelection()
{
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::AfterSelection
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::AfterSelection()
{
    Select( selected_ );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Team_ABC& element )
{
    selected_ = &element;
}
