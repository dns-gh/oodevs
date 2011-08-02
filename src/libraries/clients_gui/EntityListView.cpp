// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityListView.h"
#include "moc_EntityListView.cpp"
#include "ItemFactory_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityListView constructor
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
EntityListView::EntityListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : ListView< EntityListView >( pParent, *this, factory )
    , controllers_( controllers )
    , profile_    ( profile )
    , factory_    ( factory )
{
    setMinimumSize( 1, 1 );
    setRootIsDecorated( true );
    setResizeMode( Q3ListView::LastColumn );
    header()->hide();

    connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( Q3ListViewItem* ) ),                     this, SLOT( OnSelectionChange( Q3ListViewItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListView destructor
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
EntityListView::~EntityListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListView::OnContextMenuRequested
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int )
{
    if( ValuedListItem* value = (ValuedListItem*)( i ) )
        value->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::OnSelectionChange
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::OnSelectionChange( Q3ListViewItem* item )
{
    if( ValuedListItem* value = (ValuedListItem*)( item ) )
        value->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::OnRequestCenter
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifySelected
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    ValuedListItem* item = entity ? FindItem( entity, firstChild() ) : 0;
    if( item )
    {
        if( item != selectedItem() )
        {
            selectAll( false );
            setSelected( item, true );
        }
        ensureItemVisible( selectedItem() );
    }
    else
        clearSelection();
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyActivated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyActivated( const kernel::Entity_ABC& entity )
{
    ValuedListItem* item = FindItem( &entity, firstChild() );
    if( item )
        ensureItemVisible( item );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyUpdated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyUpdated( const kernel::Profile_ABC& /*profile*/ )
{
    Q3ListViewItemIterator it( this );
    while( ValuedListItem* item = (ValuedListItem*)( *it ) )
    {
        if( item->IsA< const Entity_ABC >() )
        {
            const Entity_ABC& entity = *item->GetValue< const Entity_ABC >();
            item->setVisible( profile_.IsVisible( entity ) );
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyUpdated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        item->SetNamed( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyCreated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyCreated( const kernel::Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( static_cast< const kernel::Entity_ABC* >( &team ), firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( static_cast< const kernel::Entity_ABC& >( team ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyDeleted
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyDeleted( const kernel::Team_ABC& team )
{
    delete FindSibling( static_cast< const kernel::Entity_ABC* >( &team ), firstChild() );
}
