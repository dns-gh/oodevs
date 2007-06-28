// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationListView.h"

#include "moc_PopulationListView.cpp"

#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ItemFactory_ABC.h"
#include "ValuedListItem.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile  )
    : ListView< PopulationListView >( pParent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , profile_( profile )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Populations" ) );
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
// Name: PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const Population_ABC& popu )
{
    const Entity_ABC& team = popu.Get< TacticalHierarchies >().GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( team );
    }
    factory_.CreateItem( teamItem )->SetNamed( (const Entity_ABC&)popu );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationListView::NotifyUpdated( const Entity_ABC& element )
{
    if( ValuedListItem* item = FindItem( &element, firstChild() ) )
        item->SetNamed( element );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const Population_ABC& element )
{
    delete FindItem( (const Entity_ABC*)&element, firstChild() );
}

// $$$$ AGE 2006-03-22: somehow factor these things
// -----------------------------------------------------------------------------
// Name: PopulationListView::OnSelectionChange
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void PopulationListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnContextMenuRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void PopulationListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifySelected
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void PopulationListView::NotifySelected( const Entity_ABC* element )
{
    ValuedListItem* item = 0;
    if( element && ( item = FindItem( element, firstChild() ) ) )
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
// Name: PopulationListView::NotifyUpdated
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
void PopulationListView::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    QListViewItemIterator it( this );
    while( ValuedListItem* item = (ValuedListItem*)( *it ) )
    {
        if( item->IsA< const Entity_ABC >() )
        {
            const Entity_ABC& entity = *item->GetValue< const Entity_ABC >();
            item->setVisible( profile.IsVisible( entity ) );
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const kernel::Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( (const Entity_ABC*)&team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( (const Entity_ABC&)team );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const kernel::Team_ABC& team )
{
    delete FindSibling( (const Entity_ABC*)&team, firstChild() );
}
