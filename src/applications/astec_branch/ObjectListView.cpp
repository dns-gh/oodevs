// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:16 $
// $Revision: 6 $
// $Workfile: ObjectListView.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectListView.h"
#include "Object_ABC.h"
#include "Team.h"
#include "Tools.h"
#include "SelectedElement.h"
#include "ASN_Messages.h"
#include "Controller.h"
#include "ValuedListItem.h"
#include "ObjectType.h"
#include "ActionController.h"

#include "moc_ObjectListView.cpp"

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
/** @param  pParent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, Controller& controller, ActionController& actionController )
    : QListView( pParent )
    , actionController_( actionController )
    , pPopupMenu_   ( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( "Objets" );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );

    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    controller.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::OnSelectionChange( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( item->IsA< const Object_ABC* >() )
        actionController_.Select( *item->GetValue< const Object_ABC* >() );
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifyCreated( const Object_ABC& object )
{
    const Team& team = object.GetTeam();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
        teamItem = new ValuedListItem( &team, this, team.GetName().c_str() );

    const ObjectType& type = object.GetType();
    ValuedListItem* typeItem = FindSibling( &type, teamItem );
    if( ! typeItem )
        typeItem = new ValuedListItem( &type, teamItem, type.GetName().c_str() );

    new ValuedListItem( &object, typeItem, object.GetName().c_str() );
}
 
// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifyUpdated( const Object_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectListView::NotifyDeleted( const Object_ABC& object )
{
    delete FindItem( &object, firstChild() );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize ObjectListView::sizeHint() const
{
    return QSize( 230, 340 );
}
