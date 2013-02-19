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
#include "Layer_ABC.h"
#include "ListItemToolTip.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;
using namespace kernel;

namespace
{
    int ItemComparator( const ValuedListItem& item1, const ValuedListItem& item2, int /*col*/, bool /*ascending*/ )
    {
        if( !item1.IsA< const Entity_ABC >() )
            return 1;
        if( !item2.IsA< const Entity_ABC >() )
            return -1;
        const Entity_ABC* entity1 = item1.GetValueNoCheck< const Entity_ABC >();
        if( !entity1 )
            return -1;
        const Entity_ABC* entity2 = item2.GetValueNoCheck< const Entity_ABC >();
        if( !entity2 )
            return 1;
        return entity1->GetId() - entity2->GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView constructor
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
EntityListView::EntityListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile )
    : ListView< EntityListView >( pParent, *this, factory )
    , controllers_( controllers )
    , profile_    ( profile )
    , blockSelect_( false )
{
    viewport()->installEventFilter( new ListItemToolTip( viewport(), *this ) );
    setMinimumSize( 1, 1 );
    setRootIsDecorated( true );
    setResizeMode( Q3ListView::LastColumn );
    header()->hide();
    SetComparator( &ItemComparator );

    connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    (                 ) ),                     this, SLOT( OnSelectionChange() ) );
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
    if( !IsReadOnly() )
        if( ValuedListItem* value = dynamic_cast< ValuedListItem* >( i ) )
            value->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::OnSelectionChange
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void EntityListView::OnSelectionChange()
{
    if( !blockSelect_ )
        MultipleSelectionChanged();
}

// -----------------------------------------------------------------------------
// Name: EntityListView::MultipleSelectionChanged
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void EntityListView::MultipleSelectionChanged()
{
    GraphicalEntity_ABC::T_GraphicalEntities list;
    Q3ListViewItemIterator it( this );
    while( ValuedListItem* item = dynamic_cast< ValuedListItem* >( *it ) )
    {
        if( item->isSelected() && item->IsA< const Entity_ABC >() )
            list.push_back( item->GetValueNoCheck< const Entity_ABC >() );
        ++it;
    }
    controllers_.actions_.SetMultipleSelection( list );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::OnRequestCenter
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::OnRequestCenter()
{
    Q3ListViewItem* selected = 0;
    if( selectionMode() == Q3ListView::Single )
        selected = selectedItem();
    else
    {
        Q3ListViewItemIterator it( this );
        for( ; !selected && *it; ++it )
            if( ( *it )->isSelected() )
                selected = *it;
    }
    if( ValuedListItem* value = dynamic_cast< ValuedListItem* >( selected ) )
        value->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifySelectionChanged
// Created: JSR 2012-05-25
// -----------------------------------------------------------------------------
void EntityListView::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    blockSelect_ = true;
    selectAll( false );
    for( std::vector< const kernel::Entity_ABC* >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        if( *it && !IsTypeRejected( **it ) )
            if( ValuedListItem* item = FindItem( *it, firstChild() ) )
            {
                setSelected( item, true );
                ensureItemVisible( item );
            }
    }
    blockSelect_ = false;
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyActivated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyActivated( const Entity_ABC& entity )
{
    if( IsTypeRejected( entity ) )
        return;
    ValuedListItem* item = FindItem( &entity, firstChild() );
    if( item )
        ensureItemVisible( item );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyUpdated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyUpdated( const Profile_ABC& /*profile*/ )
{
    Q3ListViewItemIterator it( this );
    while( ValuedListItem* item = dynamic_cast< ValuedListItem* >( *it ) )
    {
        if( item->IsA< const Entity_ABC >() )
        {
            const Entity_ABC& entity = *item->GetValueNoCheck< const Entity_ABC >();
            item->setVisible( profile_.IsVisible( entity ) );
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyUpdated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyUpdated( const Entity_ABC& entity )
{
    if( IsTypeRejected( entity ) )
        return;
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        item->SetNamed( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyUpdated
// Created: MMC 2012-07-19
// -----------------------------------------------------------------------------
void EntityListView::NotifyUpdated( const Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( static_cast< const Entity_ABC* >( &team ), firstChild() );
    if( teamItem )
        teamItem->SetNamed( static_cast< const Entity_ABC& >( team ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyCreated
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyCreated( const Team_ABC& team )
{
    ValuedListItem* teamItem = FindSibling( static_cast< const Entity_ABC* >( &team ), firstChild() );
    if( ! teamItem )
    {
        teamItem = CreateItem( this );
        teamItem->SetNamed( static_cast< const Entity_ABC& >( team ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListView::NotifyDeleted
// Created: LGY 2011-01-05
// -----------------------------------------------------------------------------
void EntityListView::NotifyDeleted( const Team_ABC& team )
{
    delete FindSibling( static_cast< const Entity_ABC* >( &team ), firstChild() );
}
