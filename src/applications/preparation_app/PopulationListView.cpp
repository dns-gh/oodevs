// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationListView.h"
#include "moc_PopulationListView.cpp"
#include "PreparationProfile.h"
#include "ModelBuilder.h"
#include "clients_gui/ValuedDragObject.h"
#include "preparation/PopulationPositions.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::PopulationListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , modelBuilder_( modelBuilder )
    , selected_    ( controllers )
{
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        modelBuilder_.DeleteEntity( *selected_ );
        key->accept();
    }
    else
        gui::PopulationListView::keyPressEvent( key );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifySelected
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::PopulationListView::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const kernel::Team_ABC& team )
{
    gui::PopulationListView::EntityListView::NotifyCreated( team );
    gui::ValuedListItem* item = gui::FindSibling( (const kernel::Entity_ABC*)&team, firstChild() );
    if( item )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::dragObject
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
Q3DragObject* PopulationListView::dragObject()
{
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    if( dynamic_cast< const kernel::Population_ABC* >( &*selected_ ) )
    {
        const PopulationPositions* pos = static_cast< const PopulationPositions* >( selected_->Retrieve< kernel::Positions >() );
        return new gui::ValuedDragObject( pos, dynamic_cast< QWidget* >( parent() ) );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnContextMenuRequested
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void PopulationListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    gui::PopulationListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}
