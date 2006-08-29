// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentListView.h"

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::AgentListView( pParent, controllers, factory )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void AgentListView::Display( const kernel::Agent_ABC& agent, gui::ValuedListItem* item )
{
    // $$$$ SBO 2006-08-29: icone embrayé/debrayé
    gui::AgentListView::Display( agent, item );
}
