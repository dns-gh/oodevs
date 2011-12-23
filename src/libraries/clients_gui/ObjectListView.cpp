// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ObjectListView */

#include "clients_gui_pch.h"
#include "ObjectListView.h"
#include "ItemFactory_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"

#pragma warning( disable : 4355 )

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : EntityListView( pParent, controllers, factory, profile )
    , controllers_( controllers )
    , factory_    ( factory )
    , profile_    ( profile )
{
    addColumn( tr( "Objects" ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyCreated
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifyCreated( const kernel::Object_ABC& object )
{
    // $$$$ AGE 2006-10-16:
    const TacticalHierarchies* hierarchies = object.Retrieve< TacticalHierarchies >();
    if( !hierarchies ) // urban block or other object not attached to a side
        return;
    const Entity_ABC& team = hierarchies->GetTop();
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
    item->setVisible( profile_.IsVisible( object ) );
    item->SetToolTip( QString( "%1 [%2]" ).arg( object.GetName() ).arg( object.GetId() ) );
    item->setDragEnabled( true );
}

namespace
{
    void DeleteHierarchy( Q3ListViewItem* item )
    {
        Q3ListViewItem* parent = item ? item->parent() : 0;
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
