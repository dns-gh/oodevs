// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InhabitantListView.h"
#include "moc_InhabitantListView.cpp"
#include "PreparationProfile.h"
#include "ModelBuilder.h"
#include "clients_gui/ValuedDragObject.h"
#include "preparation/InhabitantPositions.h"

// -----------------------------------------------------------------------------
// Name: InhabitantListView constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::InhabitantListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::InhabitantListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , selected_( controllers )
    , modelBuilder_( modelBuilder )
{
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::~InhabitantListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::keyPressEvent
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
        delete (const kernel::Entity_ABC*)selected_;
    gui::InhabitantListView::keyPressEvent( key );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifySelected
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::InhabitantListView::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::OnContextMenuRequested
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::OnContextMenuRequested( QListViewItem* item, const QPoint& pos, int index )
{
    gui::InhabitantListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyCreated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const kernel::Team_ABC& team )
{
    gui::InhabitantListView::NotifyCreated( team );
    gui::ValuedListItem* item = gui::FindSibling( (const kernel::Entity_ABC*)&team, firstChild() );
    if( item )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::dragObject
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
QDragObject* InhabitantListView::dragObject()
{
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    if( dynamic_cast< const kernel::Inhabitant_ABC* >( &*selected_ ) )
    {
        const InhabitantPositions* pos = static_cast< const InhabitantPositions* >( selected_->Retrieve< kernel::Positions >() );
        return new gui::ValuedDragObject( pos );
    }
    return 0;
}
