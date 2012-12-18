// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    , modelBuilder_( modelBuilder )
    , selected_    ( controllers )
{
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
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
    {
        modelBuilder_.DeleteEntity( *selected_ );
        key->accept();
    }
    else
        gui::InhabitantListView::keyPressEvent( key );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifySelectionChanged
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void InhabitantListView::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    // TODO g�n�raliser au cas o� y'a plusieurs �l�ments
    if( elements.size() == 1 )
        selected_ = elements.front();
    else
        selected_ = 0;
    gui::InhabitantListView::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyCreated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const kernel::Team_ABC& team )
{
    gui::InhabitantListView::EntityListView::NotifyCreated( team );
    gui::ValuedListItem* item = gui::FindSibling( (const kernel::Entity_ABC*)&team, firstChild() );
    if( item )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyCreated
// Created: JSR 2012-12-18
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const kernel::Inhabitant_ABC& inhabitant )
{
    gui::InhabitantListView::NotifyCreated( inhabitant );
    if( gui::ValuedListItem* item = gui::FindItem( static_cast< const kernel::Entity_ABC* >( &inhabitant ), firstChild() ) )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::dragObject
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Q3DragObject* InhabitantListView::dragObject()
{
    if( IsDragAndDropLocked() )
        return 0;
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    if( dynamic_cast< const kernel::Inhabitant_ABC* >( &*selected_ ) )
    {
        const InhabitantPositions* pos = static_cast< const InhabitantPositions* >( selected_->Retrieve< kernel::Positions >() );
        return new gui::ValuedDragObject( pos, dynamic_cast< QWidget* >( parent() ) );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::OnContextMenuRequested
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void InhabitantListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    gui::InhabitantListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}
