// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AgentListView.h"
#include "moc_AgentListView.cpp"

#include "ValuedListItem.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

const char* AgentListView::agentMimeType_ = "agent";

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : ListView< AgentListView >( pParent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , currentTeam_( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Unités" ) );
    setRootIsDecorated( true );
    setAcceptDrops( true );
    header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Team_ABC& team )
{
    factory_.CreateItem( this )->SetNamed( team );
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
    if( item )
        Display( value, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Team_ABC& team )
{
    Update( team );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Team_ABC& team )
{
    delete FindItem( &team, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const KnowledgeGroup_ABC& group )
{
    delete FindItem( &group, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Agent_ABC& agent )
{
    delete FindItem( &agent, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const KnowledgeGroup_ABC& group )
{
    Update( group );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Agent_ABC& agent )
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
    DeleteTail( ListView< AgentListView >::Display( value.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Team_ABC& team, ValuedListItem* item )
{
    item->SetNamed( team );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    RecursiveDisplay< Team_ABC, KnowledgeGroup_ABC >( team, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const KnowledgeGroup_ABC& group, ValuedListItem* item )
{
    item->SetNamed( group );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    RecursiveDisplay< KnowledgeGroup_ABC, Agent_ABC >( group, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Agent_ABC& agent, ValuedListItem* item )
{
    item->SetNamed( agent );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    RecursiveDisplay< Agent_ABC, Agent_ABC >( agent, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::OnSelectionChange( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void AgentListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
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
void AgentListView::Select( const Team_ABC& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const KnowledgeGroup_ABC& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const Agent_ABC& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::BeforeSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AgentListView::BeforeSelection()
{
    selectAll( false );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::AfterSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AgentListView::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void AgentListView::OptionChanged( const std::string& name, const OptionVariant& value )
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

// -----------------------------------------------------------------------------
// Name: AgentListView::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
QDragObject* AgentListView::dragObject()
{
    QListViewItem* pItem = selectedItem();
    if( !pItem )
        return 0;

    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)&pItem, sizeof( QListViewItem* ) );
    QStoredDrag* data = new QStoredDrag( agentMimeType_, this );
    data->setEncodedData( *pBytes );
    return data;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::dropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void AgentListView::dropEvent( QDropEvent* pEvent )
{
    if( !pEvent->provides( agentMimeType_ ) )
         return;

    QByteArray tmp = pEvent->encodedData( agentMimeType_ );
    ValuedListItem* droppedItem = *reinterpret_cast< ValuedListItem** >( tmp.data() );
    QPoint position = viewport()->mapFromParent( pEvent->pos() );
    ValuedListItem* targetItem = (ValuedListItem*)itemAt( position );

    if( !droppedItem || !targetItem || !Drop( *droppedItem, *targetItem ) )
        pEvent->ignore();
    else
        pEvent->accept();
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
template< typename T >
bool AgentListView::DoDrop( ValuedListItem& item, ValuedListItem& target )
{
    const T& concreteItem = *item.GetValue< const T* >();
    if( target.IsA< const Agent_ABC* >() )
        return Drop( concreteItem, *target.GetValue< const Agent_ABC* >() );
    if( target.IsA< const KnowledgeGroup_ABC* >() )
        return Drop( concreteItem, *target.GetValue< const KnowledgeGroup_ABC* >() );
    if( target.IsA< const Team_ABC* >() )
        return Drop( concreteItem, *target.GetValue< const Team_ABC* >() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( ValuedListItem& item, ValuedListItem& target )
{
    if( item.IsA< const Agent_ABC* >() )
        return DoDrop< Agent_ABC >( item, target );
    if( item.IsA< const KnowledgeGroup_ABC* >() )
        return DoDrop< KnowledgeGroup_ABC >( item, target );
    if( item.IsA< const Team_ABC* >() )
        return DoDrop< Team_ABC >( item, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC&, const Agent_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC&, const KnowledgeGroup_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC&, const Team_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC&, const Agent_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC&, const KnowledgeGroup_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC&, const Team_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Team_ABC&, const Agent_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Team_ABC&, const KnowledgeGroup_ABC& )
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Team_ABC&, const Team_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::dragEnterEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( agentMimeType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentListView::NotifyActivated( const Agent_ABC& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}
