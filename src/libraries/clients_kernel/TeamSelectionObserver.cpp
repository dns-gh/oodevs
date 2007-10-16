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
#include "Entity_ABC.h"
#include "Team_ABC.h"
#include "CommunicationHierarchies.h"
#include "TacticalHierarchies.h"

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
void TeamSelectionObserver::Select( const Entity_ABC& element )
{
    const Hierarchies* hierarchies = element.Retrieve< CommunicationHierarchies >();
    if( !hierarchies )
        hierarchies = element.Retrieve< TacticalHierarchies >();
    if( hierarchies )
        selected_ = static_cast< const Team_ABC* >( & hierarchies->GetTop() );
}
