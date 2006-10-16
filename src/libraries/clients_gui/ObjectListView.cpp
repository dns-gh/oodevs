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
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
/** @param  pParent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : QListView   ( pParent )
    , controllers_( controllers )
    , factory_( factory )
    , pPopupMenu_ ( 0 )
    , currentTeam_( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Objets" ) );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    header()->hide();

    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( this, SIGNAL( doubleClicked   ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed    ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );

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
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->Select( controllers_.actions_ );
    }
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
// Name: ObjectListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifyCreated( const Object_ABC& object )
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

    factory_.CreateItem( typeItem )->SetNamed( object );
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
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifyDeleted( const Object_ABC& object )
{
    DeleteHierarchy( FindItem( &object, firstChild() ) );
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
// Name: ObjectListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void ObjectListView::Select( const Object_ABC& object )
{
    setSelected( FindItem( &object, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ObjectListView::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        currentTeam_ = value.To< const Team_ABC* >();
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        item->setVisible( ! currentTeam_ || item->Holds( currentTeam_ ) );
        item = (ValuedListItem*)( item->nextSibling() );
    }
}
