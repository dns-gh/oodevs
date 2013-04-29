// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::HierarchyListView_ABC */

#include "clients_gui_pch.h"
#include "HierarchyListView_ABC.h"
#include "moc_HierarchyListView_ABC.cpp"
#include "EntitySymbols.h"
#include "Layer_ABC.h"
#include "ListItemToolTip.h"
#include "ValuedDragObject.h"
#include "ValuedListItem.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Ghost_ABC.h"
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
    , profile_    ( profile )
    , symbols_    ( symbols )
    , selected_   ( controllers_ )
    , activated_  ( true )
{
    viewport()->installEventFilter( new ListItemToolTip( viewport(), *this ) );

    timer_ = new QTimer( this );

    setMinimumSize( 1, 1 );
    addColumn( tr( "Units" ) );
    setRootIsDecorated( true );
    setAcceptDrops( true );
    setDragAutoScroll( true );
    viewport()->setAcceptDrops( true );
    header()->hide();

    connect( this,   SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this,   SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this,   SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this,   SIGNAL( selectionChanged    (                 ) ),                     this, SLOT( OnSelectionChange() ) );
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
        item = superiorItem ? CreateItem( superiorItem ) : CreateItem( this );
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
    if( !item )
        return;
    const bool isVisible = profile_.IsVisible( entity );
    item->SetNamed( entity );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    item->SetToolTip( QString( "%1 [%L2]" ).arg( entity.GetName() ).arg( entity.GetId() ) );
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
    if( !item )
        return;
    QPixmap pixmap = symbols_.GetSymbol( entity );
    if( pixmap.isNull() )
    {
        timer_->start( 500, true );
        return;
    }
    item->setPixmap( 0, symbols_.GetSymbol( entity, pixmap ) );
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
// Name: HierarchyListView_ABC::ClearSelection
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::ClearSelection()
{
    clearSelection();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnSelectionChange()
{
    GraphicalEntity_ABC::T_GraphicalEntities list;
    Q3ListViewItemIterator it( this );
    while( ValuedListItem* item = dynamic_cast< ValuedListItem* >( *it ) )
    {
        if( item->isSelected() && item->IsA< const Entity_ABC >() )
            list.push_back( item->GetValueNoCheck< const Entity_ABC >() );
        ++it;
    }
    if( list.size() > 0 )
        controllers_.actions_.SetMultipleSelection( list );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int )
{
    if( item && !IsReadOnly() && !IsContextMenuBlocked() )
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
    blockSignals( true );
    selectAll( false );
    blockSignals( false );

    ValuedListItem* item = element ? FindItem( element, firstChild() ) : 0;
    if( item )
    {
        selected_ = element;
        if( activated_ )
        {
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
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::NotifyActivated( const Entity_ABC& element )
{
    if( !activated_ )
        return;
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
            const Entity_ABC& entity = *root->GetValueNoCheck< const Entity_ABC >();
            DisplayIcon( entity, root );
            SetVisible( root, profile_.IsVisible( entity ) );
        }
        root = static_cast< ValuedListItem* >( root->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
Q3DragObject* HierarchyListView_ABC::dragObject()
{
    if( IsReadOnly() || IsDragAndDropLocked() )
        return 0;
    ListView< HierarchyListView_ABC >::dragObject();
    ValuedListItem* pItem = static_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    return new ValuedDragObject( pItem->GetValue< const Entity_ABC >(), this );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::viewportDragEnterEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::viewportDragEnterEvent( QDragEnterEvent* pEvent )
{
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    ListView< HierarchyListView_ABC >::viewportDragEnterEvent( pEvent );
    pEvent->accept( ValuedDragObject::Provides< const Entity_ABC >( pEvent ) ||
                    ValuedDragObject::Provides< const AgentType >( pEvent ) ||
                    ValuedDragObject::Provides< const AutomatType >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::viewportDragMoveEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::viewportDragMoveEvent( QDragMoveEvent* pEvent )
{
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    ListView< HierarchyListView_ABC >::viewportDragMoveEvent( pEvent );
    if( const Entity_ABC* entity = gui::ValuedDragObject::GetValue< Entity_ABC >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        pEvent->accept( CanDrop( entity, position ) && ValuedDragObject::Provides< const Entity_ABC >( pEvent ) );
        return;
    }
    else if( const AgentType* type = gui::ValuedDragObject::GetValue< AgentType >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        pEvent->accept( CanDropOnGhost( type, position, eGhostType_Agent ) && ValuedDragObject::Provides< const AgentType >( pEvent ) );
        return;
    }
    else if( const AutomatType* type = gui::ValuedDragObject::GetValue< AutomatType >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        pEvent->accept( CanDropOnGhost( type, position, eGhostType_Automat ) && ValuedDragObject::Provides< const AutomatType >( pEvent ) );
        return;
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::viewportDropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::viewportDropEvent( QDropEvent* pEvent )
{
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    ListView< HierarchyListView_ABC >::viewportDropEvent( pEvent );
    if( const Entity_ABC* entity = ValuedDragObject::GetValue< const Entity_ABC >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        ValuedListItem* targetItem = static_cast< ValuedListItem* >( itemAt( position ) );
        if( !targetItem || !Drop( *entity, *targetItem ) )
            pEvent->ignore();
        else
        {
            pEvent->accept();
            setFocus();
            entity->Select( controllers_.actions_ );
        }
    }
    else if( const AgentType* type = ValuedDragObject::GetValue< const AgentType >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        ValuedListItem* targetItem = static_cast< ValuedListItem* >( itemAt( position ) );
        if( !targetItem || !Drop( *type, *targetItem ) )
            pEvent->ignore();
        else
            pEvent->accept();
    }
    else if( const AutomatType* type = ValuedDragObject::GetValue< const AutomatType >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        ValuedListItem* targetItem = static_cast< ValuedListItem* >( itemAt( position ) );
        if( !targetItem || !Drop( *type, *targetItem ) )
            pEvent->ignore();
        else
            pEvent->accept();
    }
    else
        pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const Entity_ABC& entity, ValuedListItem& target )
{
    if( IsReadOnly() )
        return false;
    return target.IsA< const Entity_ABC >()
        && Drop( entity, *target.GetValueNoCheck< const Entity_ABC >() );
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
// Name: HierarchyListView_ABC::Drop
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
template< typename T>
bool HierarchyListView_ABC::Drop( const T& type, ValuedListItem& target )
{
    if( IsReadOnly() )
        return false;
    if( target.IsA< const Entity_ABC >() )
        return Drop( type, const_cast< Entity_ABC& >( *target.GetValueNoCheck< const Entity_ABC >() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const kernel::AgentType&, kernel::Entity_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::Drop
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::Drop( const kernel::AutomatType&, kernel::Entity_ABC& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::CanChangeSuperior
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::CanChangeSuperior( const kernel::Entity_ABC& /*entity*/, const kernel::Entity_ABC& /*superior*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::CanDrop
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::CanDrop( const kernel::Entity_ABC* entity, QPoint position ) const
{
    ValuedListItem* item = static_cast< ValuedListItem* >( itemAt( position ) );
    if( !entity || !item )
        return false;
    if( !item->IsA< const Entity_ABC >() )
        return false;
    const Entity_ABC* target = item->GetValue< const Entity_ABC >();
    if( !target )
        return false;
    if( entity->GetId() == target->GetId() )
        return false;
    return CanChangeSuperior( *entity, *target );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::CanDropOnGhost
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
template< typename T >
bool HierarchyListView_ABC::CanDropOnGhost( const T* type, QPoint position, E_GhostType ghostType ) const
{
    ValuedListItem* item = static_cast< ValuedListItem* >( itemAt( position ) );
    if( !type || !item )
        return false;
    if( !item->IsA< const Entity_ABC >() )
        return false;
    if( const Entity_ABC* target = item->GetValue< const Entity_ABC >() )
        if( const Ghost_ABC* ghost = dynamic_cast< const Ghost_ABC* >( target ) )
            return ghost->GetGhostType() == ghostType;
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::ActivateSelection
// Created: JSR 2012-07-06
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::ActivateSelection( bool activate )
{
    if( activate && !activated_ )
    {
        activated_ = true;
        SetContextMenuBlocked( false );
        setSelectionMode( Q3ListView::Single );
    }
    else if( !activate && activated_ )
    {
        activated_ = false;
        ClearSelection();
        setSelectionMode( Q3ListView::NoSelection );
        SetContextMenuBlocked( true );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::IsActivated
// Created: JSR 2012-07-11
// -----------------------------------------------------------------------------
bool HierarchyListView_ABC::IsActivated() const
{
    return activated_;
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::focusInEvent
// Created: LGY 2012-06-29
// -----------------------------------------------------------------------------
void HierarchyListView_ABC::focusInEvent( QFocusEvent* event )
{
    ListView< HierarchyListView_ABC >::focusInEvent( event );
    emit ListViewFocusIn( this );
}