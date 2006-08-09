// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "KnowledgeGroupSelectionObserver.h"
#include "astec_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
KnowledgeGroupSelectionObserver::KnowledgeGroupSelectionObserver()
    : selected_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver destructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
KnowledgeGroupSelectionObserver::~KnowledgeGroupSelectionObserver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::BeforeSelection
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::BeforeSelection()
{
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::AfterSelection
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::AfterSelection()
{
    Select( selected_ );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::Select( const KnowledgeGroup_ABC& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::Select( const Agent_ABC& element )
{
    selected_ = & element.GetKnowledgeGroup();
}
