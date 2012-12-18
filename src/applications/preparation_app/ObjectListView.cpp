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
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
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
    if( IsDragAndDropLocked() )
        return 0;
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
// Name: ObjectListView::NotifyCreated
// Created: JSR 2012-12-18
// -----------------------------------------------------------------------------
void ObjectListView::NotifyCreated( const kernel::Object_ABC& object )
{
    gui::ObjectListView::NotifyCreated( object );
    if( gui::ValuedListItem* item = gui::FindItem( static_cast< const kernel::Entity_ABC* >( &object ), firstChild() ) )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifySelectionChanged
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    // TODO généraliser au cas où y'a plusieurs éléments
    if( elements.size() == 1 )
        selected_ = elements.front();
    else
        selected_ = 0;
    gui::ObjectListView::NotifySelectionChanged( elements );
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
