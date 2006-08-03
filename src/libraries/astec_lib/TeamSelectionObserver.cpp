// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TeamSelectionObserver.h"
#include "KnowledgeGroup.h"
#include "Agent_ABC.h"
#include "Object.h"
#include "Population.h"

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
void TeamSelectionObserver::Select( const Team& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const KnowledgeGroup& element )
{
    Select( element.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Agent_ABC& element )
{
    Select( element.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Object& element )
{
    Select( element.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: TeamSelectionObserver::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void TeamSelectionObserver::Select( const Population& element )
{
    Select( element.GetTeam() );
}
