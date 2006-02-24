// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "KnowledgeGroupSelectionObserver.h"
#include "Agent.h"

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
void KnowledgeGroupSelectionObserver::Select( const KnowledgeGroup& element )
{
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::Select( const Agent& element )
{
    selected_ = element.GetKnowledgeGroup();
}
