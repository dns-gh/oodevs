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
    , selected_( controllers )
    , modelBuilder_( modelBuilder )
{
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
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
        delete (const kernel::Entity_ABC*)selected_;
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
// Name: PopulationListView::OnContextMenuRequested
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::OnContextMenuRequested( QListViewItem* item, const QPoint& pos, int index )
{
    gui::PopulationListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const kernel::Team_ABC& team )
{
    gui::PopulationListView::NotifyCreated( team );
    gui::ValuedListItem* item = gui::FindSibling( (const kernel::Entity_ABC*)&team, firstChild() );
    if( item )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::dragObject
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
QDragObject* PopulationListView::dragObject()
{
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    if( dynamic_cast< const kernel::Population_ABC* >( &*selected_ ) )
    {
        const PopulationPositions* pos = static_cast< const PopulationPositions* >( selected_->Retrieve< kernel::Positions >() );
        return new gui::ValuedDragObject( pos );
    }
    return 0;
}
