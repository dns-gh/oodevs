// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IntelligenceListView.h"
#include "moc_IntelligenceListView.cpp"

#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ItemFactory_ABC.h"
#include "ValuedListItem.h"
#include "EntitySymbols.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: IntelligenceListView constructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
IntelligenceListView::IntelligenceListView( QWidget* parent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, EntitySymbols& symbols )
    : ListView< IntelligenceListView >( parent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , profile_( profile )
    , symbols_( symbols )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Intelligences" ) );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    setAcceptDrops( true );
    header()->hide();

    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( Update() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );

    controllers_.Register( *this );
}
// -----------------------------------------------------------------------------
// Name: IntelligenceListView destructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
IntelligenceListView::~IntelligenceListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::OnSelectionChange
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::OnRequestCenter
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::OnContextMenuRequested
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifyCreated
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyCreated( const kernel::Intelligence_ABC& element )
{
    const Entity_ABC& team = element.Get< TacticalHierarchies >().GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( team );
    }
    ValuedListItem* elementItem = factory_.CreateItem( teamItem );
    Display( element, elementItem );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifyDeleted
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyDeleted( const kernel::Intelligence_ABC& element )
{
     delete FindItem( (const Intelligence_ABC*)&element, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifySelected
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifySelected( const kernel::Entity_ABC* element )
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
// Name: IntelligenceListView::NotifyUpdated
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    QListViewItemIterator it( this );
    while( ValuedListItem* item = (ValuedListItem*)( *it ) )
    {
        if( item->IsA< const Intelligence_ABC >() )
        {
            const Entity_ABC& entity = *item->GetValue< const Intelligence_ABC >();
            item->setVisible( profile.IsVisible( entity ) );
        }
        else if( item->IsA< const Entity_ABC >() )
        {
            const Entity_ABC& entity = *item->GetValue< const Entity_ABC >();
            item->setVisible( profile.IsVisible( entity ) );
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifyCreated
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyCreated( const kernel::Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( (const Entity_ABC*)&team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( (const Entity_ABC&)team );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifyDeleted
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyDeleted( const kernel::Team_ABC& team )
{
    delete FindSibling( (const Entity_ABC*)&team, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::Update
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
void IntelligenceListView::Update( QListViewItem* rootItem /*=0*/ )
{
    ValuedListItem* item = static_cast< ValuedListItem* >( rootItem ? rootItem->firstChild() : firstChild() );
    while( item )
    {
        if( item->IsA< const Intelligence_ABC >() )
        {
            const Intelligence_ABC& entity = *item->GetValue< const Intelligence_ABC >();
            Display( entity, item );
        }
        Update( item );
        item = static_cast< ValuedListItem* >( item->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::Display
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
void IntelligenceListView::Display( const Intelligence_ABC& entity, ValuedListItem* item )
{
    if( !item )
        return;
    item->SetNamed( entity );
    item->setDragEnabled( true );
    QPixmap pixmap = symbols_.GetSymbol( entity );
    if( pixmap.isNull() )
        timer_->start( 500, true );
    item->setPixmap( 0, pixmap );
}
