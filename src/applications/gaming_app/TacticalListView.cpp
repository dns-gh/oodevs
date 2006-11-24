// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalListView.h"
#include "gaming/AutomatDecisions.h"
#include "clients_kernel/Automat_ABC.h"

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, profile, icons )
{
    addColumn( "Embraye" );
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 1, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    else
        item->setPixmap( 1, QPixmap() );
    gui::HierarchyListView< kernel::TacticalHierarchies >::Display( agent, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const kernel::Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}
