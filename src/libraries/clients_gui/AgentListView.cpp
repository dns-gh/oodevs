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
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Hierarchies.h"
#include "ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

const char* AgentListView::agentMimeType_ = "agent"; // $$$$ AGE 2006-09-20: pas vraiment agent. Plus ValuedListItem

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
// Name: AgentListView::RecursiveCreateHierarchy
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
ValuedListItem* AgentListView::RecursiveCreateHierarchy( const Entity_ABC* entity )
{
    if( !entity )
        return 0;
    ValuedListItem* item = FindItem( entity, firstChild() );
    if( item )
        return item;
    if( const Hierarchies* hierarchy = entity->Retrieve< Hierarchies >() )
    {
        item = RecursiveCreateHierarchy( hierarchy->GetSuperior() );
        if( !item )
            item = factory_.CreateItem( this );
        else
            item = factory_.CreateItem( item );
        item->SetNamed( *entity );
        return item;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Hierarchies& hierarchy )
{
    RecursiveCreateHierarchy( &hierarchy.GetEntity() );
    NotifyUpdated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        Display( hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    delete FindItem( &entity, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    if( const Hierarchies* hierarchy = entity.Retrieve< Hierarchies >() )
        Display( *hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::Display( const Hierarchies& hierarchy, ValuedListItem* item )
{
    item->SetNamed( hierarchy.GetEntity() );
    item->setDropEnabled( true );
    item->setDragEnabled( true );

    DeleteTail( ListView< AgentListView >::Display( hierarchy.CreateSubordinateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void AgentListView::OnRequestCenter()
{
    if( ValuedListItem* item = (ValuedListItem*)( selectedItem() ) )
        item->Activate( controllers_.actions_ );
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
// Name: AgentListView::NotifySelected
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::NotifySelected( const Entity_ABC* element )
{
    selectAll( false );
    ValuedListItem* item = 0;
    if( element && ( item = FindItem( element, firstChild() ) ) )
    {
        setSelected( item, true );
        ensureItemVisible( selectedItem() );
    }
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
// Name: AgentListView::dragEnterEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( agentMimeType_ ) );
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
bool AgentListView::Drop( ValuedListItem& item, ValuedListItem& target )
{
    return item.IsA< const Entity_ABC* >()
        && target.IsA< const Entity_ABC* >()
        && Drop( *item.GetValue< const Entity_ABC* >(), *target.GetValue< const Entity_ABC* >() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Entity_ABC& , const Entity_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentListView::NotifyActivated( const Entity_ABC& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}
