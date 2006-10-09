// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;

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
    Select( static_cast< const kernel::KnowledgeGroup_ABC* >( selected_ ) );
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
    selected_ = & element.Get< CommunicationHierarchies >().GetUp( 2 );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupSelectionObserver::Select
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void KnowledgeGroupSelectionObserver::Select( const kernel::Automat_ABC& element )
{
    selected_ = & element.Get< CommunicationHierarchies >().GetUp();
}
