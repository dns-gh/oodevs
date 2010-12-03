// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::InhabitantListView */

#include "clients_gui_pch.h"
#include "InhabitantListView.h"
#include "moc_InhabitantListView.cpp"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ItemFactory_ABC.h"
#include "ValuedListItem.h"

#pragma warning( disable : 4355 )

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantListView constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::InhabitantListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile  )
    : ListView< InhabitantListView >( pParent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , profile_( profile )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Inhabitants" ) );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    setAcceptDrops( true );
    header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::~InhabitantListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyCreated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const Inhabitant_ABC& popu )
{
    const Entity_ABC& team = popu.Get< TacticalHierarchies >().GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( team );
    }
    ValuedListItem* popItem = factory_.CreateItem( teamItem );
    popItem->SetNamed( (const Entity_ABC&)popu );
    popItem->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyUpdated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyUpdated( const Entity_ABC& element )
{
    if( ValuedListItem* item = FindItem( &element, firstChild() ) )
        item->SetNamed( element );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyDeleted
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyDeleted( const Inhabitant_ABC& element )
{
    delete FindItem( (const Entity_ABC*)&element, firstChild() );
}

// $$$$ AGE 2006-03-22: somehow factor these things
// -----------------------------------------------------------------------------
// Name: InhabitantListView::OnSelectionChange
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::OnRequestCenter
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::OnContextMenuRequested
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifySelected
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifySelected( const Entity_ABC* element )
{
    ValuedListItem* item = element ? FindItem( element, firstChild() ) : 0;
    if( item )
    {
        if( item != selectedItem() )
        {
            selectAll( false );
            setSelected( item, true );
        }
        ensureItemVisible( selectedItem() );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyUpdated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyUpdated( const kernel::Profile_ABC& )
{
    QListViewItemIterator it( this );
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
// Name: InhabitantListView::NotifyCreated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const kernel::Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( (const Entity_ABC*)&team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( (const Entity_ABC&)team );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyDeleted
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyDeleted( const kernel::Team_ABC& team )
{
    delete FindSibling( (const Entity_ABC*)&team, firstChild() );
}
