// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::HierarchyListView_ABC */

#include "clients_gui_pch.h"
#include "HierarchyListView_ABC.h"
#include "moc_HierarchyListView_ABC.cpp"
#include "EntitySymbols.h"
#include "ItemFactory_ABC.h"
#include "ListItemToolTip.h"
#include "ValuedDragObject.h"
#include "ValuedListItem.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

#pragma warning( disable : 4355 )

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
HierarchyListView_ABC::HierarchyListView_ABC( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile, const EntitySymbols& symbols )
    : ListView< HierarchyListView_ABC >( pParent, *this, factory, "HierarchyListView" )
    , controllers_( controllers )
    , factory_    ( factory )
    , profile_    ( profile )
    , symbols_    ( symbols )
    , selected_   ( controllers_ )
{
    viewport()->installEventFilter( new ListItemToolTip( viewport(), *this ) );

    timer_ = new QTimer( this );

    setMinimumSize( 1, 1 );
    addColumn( tr( "Units" ) );
    setRootIsDecorated( true );
    setAcceptDrops( true );
    setDragAutoScroll( true );
//    viewport()->setAcceptDrops( true ); // $$$$ SBO 2010-08-12: needed to enable autoscroll
    header()->hide();

    connect( this,   SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this,   SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this,   SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this,   SIGNAL( selectionChanged    ( Q3ListViewItem* ) ),                     this, SLOT( OnSelectionChange( Q3ListViewItem* ) ) );
    connect( timer_, SIGNAL( timeout() ),                                                   this, SLOT( Update() ) );
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
void HierarchyListView_ABC::NotifyCreated( const Hierarchies& hierarchy )
{
    NotifyUpdated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyUpdated
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyUpdated( const Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    Display( entity, FindOrCreate( &entity ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyDeleted
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyDeleted( const Hierarchies& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    delete FindItem( &entity, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::RecursiveCreateHierarchy
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
ValuedListItem* HierarchyListView_ABC::FindOrCreate( const Entity_ABC* entity )
{
    if( !entity )
        return 0;
    const Hierarchies* hierarchy = RetrieveHierarchy( *entity );
    const Entity_ABC* superior = hierarchy ? hierarchy->GetSuperior() : 0;
    ValuedListItem* superiorItem = FindOrCreate( superior );
    ValuedListItem* item = superiorItem ? FindChild( entity, superiorItem ) : FindSibling( entity, firstChild() );
    if( !item )
    {
        item = superiorItem ? factory_.CreateItem( superiorItem ) : factory_.CreateItem( this );
        UpdateItem( item );
    }
    return item;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    const bool isVisible = profile_.IsVisible( entity );
    item->SetNamed( entity );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    item->SetToolTip( QString( "%1 [%2]" ).arg( entity.GetName() ).arg( entity.GetId() ) );
    if( const Hierarchies* hierarchy = RetrieveHierarchy( entity ) )
        DeleteTail( ListView< HierarchyListView_ABC >::Display( hierarchy->CreateSubordinateIterator(), item ) );
    DisplayIcon( entity, item );
    SetVisible( item, isVisible );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::DisplayIcon
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::DisplayIcon( const Entity_ABC& entity, ValuedListItem* item )
{
    QPixmap pixmap = symbols_.GetSymbol( entity );
    if( pixmap.isNull() )
    {
        timer_->start( 500, true );
        return;
    }
    item->setPixmap( 0, pixmap );
    item->invalidateHeight();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::SetVisible
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::SetVisible( Q3ListViewItem* item, bool visible )
{
    if( item )
    {
        item->setVisible( visible );
        if( visible )
            SetVisible( item->parent(), visible );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::ApplyFilter
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::ApplyFilter( boost::function< bool ( gui::ValuedListItem* ) > func )
{
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
    {
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( it.current() );
        item->setVisible( HasAnyChildVisible( item, func ) );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::HasAnyChildVisible
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::HasAnyChildVisible( gui::ValuedListItem* item, boost::function< bool ( gui::ValuedListItem* ) > func )
{
    if( item )
    {
        bool isVisible = func( item );
        gui::ValuedListItem* child = static_cast< gui::ValuedListItem* >( item->firstChild() );
        if( child )
            for( Q3ListViewItemIterator it = child; it.current(); ++it )
                if( static_cast< gui::ValuedListItem* >( it.current()->parent() ) == item )
                    isVisible = isVisible || HasAnyChildVisible( static_cast< gui::ValuedListItem* >( it.current() ), func );
        return isVisible;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnSelectionChange( Q3ListViewItem* item )
{
    if( item )
        static_cast< ValuedListItem* >( item )->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int )
{
    if( item )
        static_cast< ValuedListItem* >( item )->ContextMenu( controllers_.actions_, pos );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnRequestCenter()
{
    ValuedListItem* item = static_cast< ValuedListItem* >( selectedItem() );
    if( item )
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
    ValuedListItem* item = element ? FindItem( element, firstChild() ) : 0;
    if( item )
    {
        selected_ = element;
        if( item != selectedItem() )
        {
            blockSignals( true );
            selectAll( false );
            setSelected( item, true );
            blockSignals( false );
        }
        ensureItemVisible( selectedItem() );
    }
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

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyUpdated
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyUpdated( const Profile_ABC& )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyUpdated
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyUpdated( const Symbol_ABC& symbol )
{
    const Entity_ABC& entity = symbol.GetEntity();
    ValuedListItem* item = FindItem( &entity, firstChild() );
    if( item )
        DisplayIcon( entity, item );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Update
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::Update()
{
    UpdateItem( static_cast< ValuedListItem* >( firstChild() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::UpdateItem
// Created: SBO 2010-08-03
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::UpdateItem( ValuedListItem* root )
{
    while( root )
    {
        UpdateItem( static_cast< ValuedListItem* >( root->firstChild() ) );
        if( root->IsA< const Entity_ABC >() )
        {
            const Entity_ABC& entity = *root->GetValue< const Entity_ABC >();
            DisplayIcon( entity, root );
            SetVisible( root, profile_.IsVisible( entity ) );
        }
        root = static_cast< ValuedListItem* >( root->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::focusInEvent
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::focusInEvent( QFocusEvent* event )
{
    ListView< HierarchyListView_ABC >::focusInEvent( event );
    if( selected_ )
    {
        ValuedListItem* item = FindItem( &*selected_, firstChild() );
        if( item )
            item->Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
Q3DragObject* HierarchyListView_ABC::dragObject()
{
    ValuedListItem* pItem = static_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    return new ValuedDragObject( pItem->GetValue< const Entity_ABC >(), this );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dragEnterEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( ValuedDragObject::Provides< const Entity_ABC >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dragMoveEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::dragMoveEvent( QDragMoveEvent* pEvent )
{
    pEvent->accept( ValuedDragObject::Provides< const Entity_ABC >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::dropEvent( QDropEvent* pEvent )
{
    const Entity_ABC* entity = ValuedDragObject::GetValue< const Entity_ABC >( pEvent );
    if( entity )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        ValuedListItem* targetItem = static_cast< ValuedListItem* >( itemAt( position ) );
        if( !entity || !targetItem || !Drop( *entity, *targetItem ) )
            pEvent->ignore();
        else
            pEvent->accept();
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const Entity_ABC& entity, ValuedListItem& target )
{
    return target.IsA< const Entity_ABC >()
        && Drop( entity, *target.GetValue< const Entity_ABC >() );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const Entity_ABC& , const Entity_ABC& )
{
    return false;
}
