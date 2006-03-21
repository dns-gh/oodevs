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

#include "astec_pch.h"
#include "AgentListView.h"
#include "Controller.h"
#include "ActionController.h"
#include "ValuedListItem.h"

#include "Agent.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "AutomatDecisions.h"

#include "moc_AgentListView.cpp"

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
/** @param  pParent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controller& controller, ActionController& actionController )
    : ListView< AgentListView >( pParent, *this )
    , actionController_( actionController )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    setMinimumSize( 1, 1 );
    addColumn( "Unités" );
    setRootIsDecorated( true );
    setAcceptDrops( true );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
//    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
//    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    controller.Register( *this );
    actionController.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    // $$$$ AGE 2006-03-16: controller_.Remove
    // NOTHING
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
// Name: AgentListView::Update
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename T >
void AgentListView::Update( const T& value )
{
    ValuedListItem* item = FindItem( &value, firstChild() );
    if( ! item )
        throw std::runtime_error( std::string( "Could not update item " ) + typeid( value ).name() + " '" + value.GetName() + "'" );
    Display( value, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Team& team )
{
    Update( team );
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
    Update( group );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Agent& agent )
{
    Update( agent );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::RecursiveDisplay
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename ParentType, typename ChildType >
void AgentListView::RecursiveDisplay( const ParentType& value, ValuedListItem* item )
{
    DeleteTail( ListView< AgentListView >::Display( value.CreateIterator(), item, (ValuedListItem*)( item->firstChild() ) ) );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Team& team, ValuedListItem* item )
{
    item->SetValue( &team );
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
    item->setText( 0, group.GetName().c_str() );
    RecursiveDisplay< KnowledgeGroup, Agent >( group, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Agent& agent, ValuedListItem* item )
{
    item->SetValue( &agent );
    item->setText( 0, agent.GetName().c_str() );
    if( agent.Retrieve< AutomatDecisions >() )
    {
        const QPixmap pix = agent.Get< AutomatDecisions >().IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye );
        item->setPixmap( 0, pix );
        RecursiveDisplay< Agent, Agent >( agent, item );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    ValuedListItem* item = FindItem( & decisions.GetAgent(), firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::OnSelectionChange( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    item->Select( actionController_ );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    pPopupMenu_->clear();

    ValuedListItem* item = (ValuedListItem*)( i );
    item->ContextMenu( actionController_, *pPopupMenu_ );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const Team& element )
{
   setSelected( FindItem( &element, firstChild() ), true );
   ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const KnowledgeGroup& element )
{
   setSelected( FindItem( &element, firstChild() ), true );
   ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const Agent& element )
{
   setSelected( FindItem( &element, firstChild() ), true );
   ensureItemVisible( selectedItem() );
}
