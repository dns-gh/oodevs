// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamSelectionObserver.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

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
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Team_ABC& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const KnowledgeGroup_ABC& element )
{
    Select( element.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Agent_ABC& element )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( element.Get< CommunicationHierarchies >().GetTop() );
    Select( team );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Object_ABC& element )
{
    Select( element.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Population_ABC& element )
{
    Select( element.GetTeam() );
}
