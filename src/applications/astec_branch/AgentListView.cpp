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
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
        throw std::runtime_error( "up" );
    Display( team, teamItem );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Team& team )
{
    delete FindChild( &team, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const KnowledgeGroup& group )
{
    ValuedListItem* groupItem = FindItem( &group, firstChild() );
    if( ! groupItem )
        throw std::runtime_error( "up" );
    Display( group, groupItem );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Agent& agent )
{
    ValuedListItem* agentItem = FindItem( &agent, firstChild() );
    if( ! agentItem )
        throw std::runtime_error( "up" );
    Display( agent, agentItem );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::RecursiveDisplay
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename ParentType, typename ChildType >
void AgentListView::RecursiveDisplay( const ParentType& value, ValuedListItem* item )
{
    ValuedListItem* previousChild = (ValuedListItem*)( item->firstChild() );

    Iterator< const ChildType& > it = value.CreateIterator();
    ValuedListItem* child = previousChild;
    while( it.HasMoreElements() )
    {
        const ChildType& childValue = it.NextElement();
        if( ! child  ) 
            child = new ValuedListItem( &childValue, item, previousChild );
        Display( childValue, child );
        previousChild = child;
        child = (ValuedListItem*)( child->nextSibling() );
    }
    while( child )
    {
        ValuedListItem* next = (ValuedListItem*)( child->nextSibling() );
        delete child;
        child = next;
    }
}


// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Team& team, ValuedListItem* item )
{
    item->setText( 0, team.GetName().c_str() );
    RecursiveDisplay< Team, KnowledgeGroup >( team, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const KnowledgeGroup& group, ValuedListItem* item )
{
    item->SetValue( &group );
    item->setText( 0, QString( "Gtia %1" ).arg( group.GetId() ) );

    RecursiveDisplay< KnowledgeGroup, Agent >( group, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Agent& automat, ValuedListItem* item )
{
    item->setText( 0, automat.GetName().c_str() );
    RecursiveDisplay< Agent, Agent >( automat, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}
