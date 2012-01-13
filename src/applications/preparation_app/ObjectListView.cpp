// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectListView.h"
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "clients_gui/ValuedDragObject.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/tools.h"

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::ObjectListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , modelBuilder_( modelBuilder )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::dragObject
// Created: JSR 2011-12-23
// -----------------------------------------------------------------------------
Q3DragObject* ObjectListView::dragObject()
{
    gui::ObjectListView::dragObject();
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    return new gui::ValuedDragObject( pItem->GetValue< const kernel::Entity_ABC >(), this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        modelBuilder_.DeleteEntity( *selected_ );
        key->accept();
    }
    else
        gui::ObjectListView::keyPressEvent( key );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifySelected
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::ObjectListView::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OnContextMenuRequested
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void ObjectListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    gui::ObjectListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tools::translate( "ObjectListView", "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}
