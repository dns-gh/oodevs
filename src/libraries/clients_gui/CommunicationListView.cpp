// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CommunicationListView.h"
#include "moc_CommunicationListView.cpp"

#include "ValuedListItem.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

const char* CommunicationListView::agentMimeType_ = "agent"; // $$$$ AGE 2006-09-20: pas vraiment agent. Plus ValuedListItem

// -----------------------------------------------------------------------------
// Name: CommunicationListView constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
CommunicationListView::CommunicationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : ListView< CommunicationListView >( pParent, *this, factory )
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
// Name: CommunicationListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
CommunicationListView::~CommunicationListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::RecursiveCreateHierarchy
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
ValuedListItem* CommunicationListView::RecursiveCreateHierarchy( const Entity_ABC* entity )
{
    if( !entity )
        return 0;
    ValuedListItem* item = FindItem( entity, firstChild() );
    if( item )
        return item;
    if( const Hierarchies* hierarchy = entity->Retrieve< CommunicationHierarchies >() )
        item = RecursiveCreateHierarchy( hierarchy->GetSuperior() );
    if( !item )
        item = factory_.CreateItem( this );
    else
        item = factory_.CreateItem( item );
    item->SetNamed( *entity );
    return item;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyCreated
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyCreated( const CommunicationHierarchies& hierarchy )
{
    RecursiveCreateHierarchy( &hierarchy.GetEntity() );
    NotifyUpdated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyUpdated
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyUpdated( const CommunicationHierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        Display( hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyDeleted
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyDeleted( const CommunicationHierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    delete FindItem( &entity, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    if( const CommunicationHierarchies* hierarchy = entity.Retrieve< CommunicationHierarchies >() )
        Display( *hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const CommunicationHierarchies& hierarchy, ValuedListItem* item )
{
    item->SetNamed( hierarchy.GetEntity() );
    item->setDropEnabled( true );
    item->setDragEnabled( true );

    DeleteTail( ListView< CommunicationListView >::Display( hierarchy.CreateSubordinateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void CommunicationListView::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void CommunicationListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void CommunicationListView::OnRequestCenter()
{
    if( ValuedListItem* item = (ValuedListItem*)( selectedItem() ) )
        item->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize CommunicationListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifySelected
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::NotifySelected( const Entity_ABC* element )
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
// Name: CommunicationListView::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void CommunicationListView::OptionChanged( const std::string& name, const OptionVariant& value )
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
// Name: CommunicationListView::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
QDragObject* CommunicationListView::dragObject()
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
// Name: CommunicationListView::dragEnterEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void CommunicationListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( agentMimeType_ ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::dropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void CommunicationListView::dropEvent( QDropEvent* pEvent )
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
// Name: CommunicationListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( ValuedListItem& item, ValuedListItem& target )
{
    return item.IsA< const Entity_ABC* >()
        && target.IsA< const Entity_ABC* >()
        && Drop( *item.GetValue< const Entity_ABC* >(), *target.GetValue< const Entity_ABC* >() );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Entity_ABC& , const Entity_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyActivated( const Entity_ABC& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}
