// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentSelectionObserver.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
AgentSelectionObserver::AgentSelectionObserver()
    : selected_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver destructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
AgentSelectionObserver::~AgentSelectionObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver::BeforeSelection
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void AgentSelectionObserver::BeforeSelection()
{
    selected_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver::AfterSelection
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void AgentSelectionObserver::AfterSelection()
{
    NotifySelected( selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver::Select
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void AgentSelectionObserver::Select( const Population_ABC& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentSelectionObserver::Select
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void AgentSelectionObserver::Select( const Agent_ABC& element )
{
    selected_ = &element;
}
