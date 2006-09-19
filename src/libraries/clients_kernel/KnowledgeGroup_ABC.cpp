// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "KnowledgeGroup_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::~KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Select
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::ContextMenu
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Activate
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}
