// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HierarchyListView_ABC.h"
#include "moc_HierarchyListView_ABC.cpp"

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

const char* HierarchyListView_ABC::agentMimeType_ = "agent"; // $$$$ AGE 2006-09-20: pas vraiment agent. Plus ValuedListItem

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
HierarchyListView_ABC::HierarchyListView_ABC( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : ListView< HierarchyListView_ABC >( pParent, *this, factory )
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
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
HierarchyListView_ABC::~HierarchyListView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyCreated
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyCreated( const kernel::Hierarchies& hierarchy )
{   
    RecursiveCreateHierarchy( &hierarchy.GetEntity() );
    NotifyUpdated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyUpdated
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyUpdated( const kernel::Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        Display( hierarchy, item );
}   

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyDeleted
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyDeleted( const kernel::Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    delete FindItem( &entity, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::RecursiveCreateHierarchy
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
ValuedListItem* HierarchyListView_ABC::RecursiveCreateHierarchy( const Entity_ABC* entity )
{
    if( !entity )
        return 0;
    ValuedListItem* item = FindItem( entity, firstChild() ); // $$$$ AGE 2006-10-11: pas terrible
    if( item )
        return item;
    if( const Hierarchies* hierarchy = RetrieveHierarchy( *entity ) )
        item = RecursiveCreateHierarchy( hierarchy->GetSuperior() );
    if( !item )
        item = factory_.CreateItem( this );
    else
        item = factory_.CreateItem( item );
    item->SetNamed( *entity );
    return item;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    if( const Hierarchies* hierarchy = RetrieveHierarchy( entity ) )
        Display( *hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::Display( const Hierarchies& hierarchy, ValuedListItem* item )
{
    item->SetNamed( hierarchy.GetEntity() );
    item->setDropEnabled( true );
    item->setDragEnabled( true );

    DeleteTail( ListView< HierarchyListView_ABC >::Display( hierarchy.CreateSubordinateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnSelectionChange( QListViewItem* i )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( ValuedListItem* item = (ValuedListItem*)( i ) )
        item->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnRequestCenter()
{
    if( ValuedListItem* item = (ValuedListItem*)( selectedItem() ) )
        item->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize HierarchyListView_ABC::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifySelected
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifySelected( const Entity_ABC* element )
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
// Name: HierarchyListView_ABC::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        currentTeam_ = value.To< const Team_ABC* >();
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        item->setVisible( ! currentTeam_ || item->Holds( (const Entity_ABC*)currentTeam_ ) );
        item = (ValuedListItem*)( item->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
QDragObject* HierarchyListView_ABC::dragObject()
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
// Name: HierarchyListView_ABC::dragEnterEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( agentMimeType_ ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::dropEvent( QDropEvent* pEvent )
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
// Name: HierarchyListView_ABC::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( ValuedListItem& item, ValuedListItem& target )
{
    return item.IsA< const Entity_ABC* >()
        && target.IsA< const Entity_ABC* >()
        && Drop( *item.GetValue< const Entity_ABC* >(), *target.GetValue< const Entity_ABC* >() );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const Entity_ABC& , const Entity_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyActivated( const Entity_ABC& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}
