// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentListViewImp.h"
#include "gaming/AutomatDecisions.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentListViewImp constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListViewImp::AgentListViewImp( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory )
    : AgentListView( pParent, controllers, publisher, factory )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentListViewImp destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListViewImp::~AgentListViewImp()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListViewImp::Display
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListViewImp::Display( const Agent_ABC& agent, ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
    {
        const QPixmap pix = decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye );
        item->setPixmap( 0, pix );
    }
    AgentListView::Display( agent, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListViewImp::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListViewImp::NotifyUpdated( const AutomatDecisions& decisions )
{
    ValuedListItem* item = FindItem( & decisions.GetAgent(), firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}
