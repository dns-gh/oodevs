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
// Name: InhabitantListView::NotifySelected
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::InhabitantListView::NotifySelected( element );
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
// Name: InhabitantListView::dragObject
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Q3DragObject* InhabitantListView::dragObject()
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
