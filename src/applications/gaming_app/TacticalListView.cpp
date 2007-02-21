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
#include "clients_kernel/CommandPostAttributes.h"
#include "clients_kernel/Automat_ABC.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, profile, icons )
{
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
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
// Name: TacticalListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 1, decisions->IsEmbraye() ? MAKE_PIXMAP( lock ) : QPixmap() );
    else if( const kernel::CommandPostAttributes* commandPost = entity.Retrieve< kernel::CommandPostAttributes >() )
        item->setPixmap( 1, commandPost->IsCommandPost() ? MAKE_PIXMAP( commandpost ) : QPixmap() );
    gui::HierarchyListView< kernel::TacticalHierarchies >::Display( entity, item );
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
        item->setPixmap( 1, decisions.IsEmbraye() ? MAKE_PIXMAP( lock ) : QPixmap() );
}
