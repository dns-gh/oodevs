// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.cpp $
// $Author: Ape $
// $Modtime: 23/11/04 18:42 $
// $Revision: 14 $
// $Workfile: AgentListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentListView.h"
//#include "moc_AgentListView.cpp"
#include "Controller.h"
#include "ValuedListItem.h"

#include "Agent.h"
#include "Team.h"
#include "KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
/** @param  pParent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controller& controller )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( "Unités" );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    setAcceptDrops( true );

//    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
//    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
//    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
//    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    controller.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Team& team )
{
    new ValuedListItem( &team, this, team.GetName().c_str() );
    NotifyUpdated( team );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Team& team )
{
    ValuedListItem* teamItem = FindChild( &team, firstChild() );
    if( ! teamItem )
        throw "up";

    ValuedListItem* child = (ValuedListItem*)( teamItem->firstChild() );
    Iterator< const KnowledgeGroup& > it = team.CreateIterator();

    while( it.HasMoreElements() )
    {
        const KnowledgeGroup& group = it.NextElement();
        if( ! child->IsA< const KnowledgeGroup* >() || child->GetValue< const KnowledgeGroup* >() != &group )
        {
            ValuedListItem* newChild = new ValuedListItem( &group, child, "stoopid group" );
            delete child;
            child = newChild;
        }
        // $$$$ AGE 2006-02-15: Display 'group' in 'child' 
        child = (ValuedListItem*)( child->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Team& team )
{
    ValuedListItem* item = FindChild( &team, firstChild() );
    if( item )
        delete item;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}
