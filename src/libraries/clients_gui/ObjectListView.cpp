// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectListView.h"
#include "moc_ObjectListView.cpp"
#include "Tools.h"
#include "ItemFactory_ABC.h"
#include "ValuedListItem.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : ListView< ObjectListView >( pParent, *this, factory )
    , controllers_( controllers )
    , factory_    ( factory )
    , profile_( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Objets" ) );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ObjectListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyCreated
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifyCreated( const kernel::Object_ABC& object )
{
   
    // $$$$ AGE 2006-10-16: 
    const Team_ABC& team = static_cast< const Team_ABC& >( object.Get< TacticalHierarchies >().GetUp() );
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( team );
    }

    const ObjectType& type = object.GetType();
    ValuedListItem* typeItem = FindChild( &type, teamItem );
    if( ! typeItem )
    {
        typeItem = factory_.CreateItem( teamItem );
        typeItem->SetNamed( type );
    }

    ValuedListItem* item = factory_.CreateItem( typeItem );
    item->SetNamed( (const Entity_ABC&)object );
    item->setVisible( profile_ && profile_->IsVisible( object ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyUpdated
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::ValuedListItem* item = gui::FindItem( (const kernel::Entity_ABC*)&entity, firstChild() );
    if( item )
        item->SetNamed( entity );
}

namespace
{
    void DeleteHierarchy( QListViewItem* item )
    {
        QListViewItem* parent = item ? item->parent() : 0;
        delete item;
        if( parent && ! parent->childCount() )
            DeleteHierarchy( parent );
    };  
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyDeleted
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifyDeleted( const kernel::Object_ABC& object )
{
    DeleteHierarchy( FindItem( (const kernel::Entity_ABC*)&object, firstChild() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize ObjectListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifySelected
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifySelected( const kernel::Entity_ABC* element )
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
// Name: ObjectListView::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void ObjectListView::NotifyActivated( const Entity_ABC& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyUpdated
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void ObjectListView::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    profile_ = &profile;

    QListViewItemIterator it( this );
    while( ValuedListItem* item = (ValuedListItem*)( *it ) )
    {
        if( item->IsA< const Entity_ABC* >() )
        {
            const Entity_ABC& entity = *item->GetValue< const Entity_ABC* >();
            item->setVisible( profile_->IsVisible( entity ) );
        }
        ++it;
    }
}
