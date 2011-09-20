// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::LogisticListView */

#include "clients_gui_pch.h"
#include "LogisticListView.h"

#include "EntitySymbols.h"
#include "HierarchyListView_ABC.h"
#include "ListView.h"
#include "ListItemToolTip.h"
#include "LongNameHelper.h"
#include "ValuedDragObject.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticListView constructor
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
LogisticListView::LogisticListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile, const EntitySymbols& symbols, bool renamable /*= false*/ )
    : HierarchyListView_ABC( pParent, controllers, factory, profile, symbols )
    , factory_    ( factory )
    , profile_    ( profile )
    , droppedUnit_( 0 )
    , renamable_  ( renamable )
{
    addColumn( tools::translate( "gui::LogisticListView", "Units" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView destructor
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
LogisticListView::~LogisticListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::FindOrCreateOrReplace
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
ValuedListItem* LogisticListView::FindOrCreateOrReplace( const Entity_ABC* entity )
{
    if( !entity )
        return 0;
    ValuedListItem* item = FindItem( entity, firstChild() );
    if( GetLogisticLevel( *entity ) == LogisticLevel::none_ && entity->GetTypeName() == Formation_ABC::typeName_ )
    {
        if( item )
            DeleteSubTree( item );
        return 0;
    }
    ValuedListItem& teamItem = RetrieveTeamItem( *entity );
    ValuedListItem& typeItem = RetrieveTypeItem( *entity, teamItem );
    const Entity_ABC* superior = RetrieveSuperior( *entity );
    ValuedListItem* superiorItem = ( superior ) ? FindItem( superior, firstChild() ) : 0;
    if( !item )
        item = ( superiorItem ) ? CreateItem( *superiorItem, *entity ) : CreateItem( typeItem, *entity );
    else if( item != superiorItem )
    {
        ValuedListItem* parentItem = static_cast< ValuedListItem* >( item->parent() );
        if( superiorItem && parentItem != superiorItem )
            item = MoveSubTree( *item, *superiorItem );
        else if( !superiorItem && parentItem != &typeItem )
            item = MoveSubTree( *item, typeItem );
    }
    UpdateItem( item );
    item->setRenameEnabled( 0, renamable_ && !LongNameHelper::SetItemLongName( *entity, *item ) );
    return item;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::GetLogisticLevel
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
const LogisticLevel& LogisticListView::GetLogisticLevel( const Entity_ABC& entity ) const
{
    if(  entity.GetTypeName() == Formation_ABC::typeName_ )
        return static_cast< const Formation_ABC* >( &entity )->GetLogisticLevel();
    else if( entity.GetTypeName() == Automat_ABC::typeName_ )
        return static_cast< const Automat_ABC* >( &entity )->GetLogisticLevel();
    assert( false );
    return LogisticLevel::none_;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::RetrieveHierarchy
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
const Hierarchies* LogisticListView::RetrieveHierarchy( const Entity_ABC& /*entity*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::CreateItem
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
ValuedListItem* LogisticListView::CreateItem( ValuedListItem& parent, const Entity_ABC& entity )
{
    ValuedListItem* item = factory_.CreateItem( &parent );
    item->SetNamed( static_cast< const Entity_ABC& >( entity ) );
    item->SetToolTip( QString( "%1 [%2]" ).arg( entity.GetName() ).arg( entity.GetId() ) );
    item->setDragEnabled( true );
    SetVisible( item, profile_.IsVisible( entity ) );
    DisplayIcon( entity, item );
    return item;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::MoveSubTree
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
ValuedListItem* LogisticListView::MoveSubTree( ValuedListItem& from, ValuedListItem& toParent )
{
    ValuedListItem* result = MoveSubTreeInternal( from, toParent );
    UpdateItem( result );
    LogisticListView::DeleteSubTree( &from );
    if( result && droppedUnit_ )
    {
        ensureItemVisible( result );
        result->Select( controllers_.actions_ );
        droppedUnit_ = 0;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::MoveSubTreeInternal
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
ValuedListItem* LogisticListView::MoveSubTreeInternal( ValuedListItem& from, ValuedListItem& toParent )
{
    ValuedListItem* newTail = CreateItem( toParent, *from.GetValue< const Entity_ABC >() );
    assert( newTail );
    ValuedListItem* item = static_cast< ValuedListItem* >( from.firstChild() );
    while( item )
    {
        MoveSubTreeInternal( *item, *newTail );
        item = static_cast< ValuedListItem* >( item->nextSibling() );
    }
    return newTail;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::DeleteTree
// Created: ABR 2011-09-15
// -----------------------------------------------------------------------------
void LogisticListView::DeleteSubTree( ValuedListItem* root )
{
    ValuedListItem* child = (ValuedListItem*) root->firstChild();
    while( child )
    {
        ValuedListItem* next = (ValuedListItem*) child->nextSibling();
        DeleteSubTree( child );
        child = next;
    }
    if( root )
        delete root;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::RetrieveTeamItem
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
ValuedListItem& LogisticListView::RetrieveTeamItem( const Entity_ABC& entity )
{
    const TacticalHierarchies& hierarchies = entity.Get< TacticalHierarchies >();
    const Entity_ABC& team = hierarchies.GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    assert( teamItem );
    return *teamItem;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::RetrieveTypeItem
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
ValuedListItem& LogisticListView::RetrieveTypeItem( const Entity_ABC& entity, ValuedListItem& teamItem )
{
    const LogisticLevel& logisticLevel = GetLogisticLevel( entity );
    ValuedListItem* typeItem = FindChild( &logisticLevel, &teamItem );
    assert( typeItem );
    return *typeItem;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyCreated
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
void LogisticListView::NotifyCreated( const Automat_ABC& automat )
{
    FindOrCreateOrReplace( &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyDeleted
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
void LogisticListView::NotifyDeleted( const Automat_ABC& automat )
{
    NotifyDeletedInternal( automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyCreated
// Created: ABR 2011-09-13
// -----------------------------------------------------------------------------
void LogisticListView::NotifyCreated( const Formation_ABC& formation )
{
    FindOrCreateOrReplace( &formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyDeleted
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
void LogisticListView::NotifyDeleted( const Formation_ABC& formation )
{
    NotifyDeletedInternal( formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyCreated
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::NotifyCreated( const Team_ABC& team )
{
    ValuedListItem* teamItem = factory_.CreateItem( this );
    teamItem->SetNamed( static_cast< const Entity_ABC& >( team ) );
    teamItem->setDragEnabled( false );
    teamItem->setRenameEnabled( 0, renamable_ && !LongNameHelper::SetItemLongName( team, *teamItem ) );

    ValuedListItem* typeItem = factory_.CreateItem( teamItem );
    typeItem->Set( &LogisticLevel::none_, tools::translate( "gui::LogisticListView", "Unsupported units" ) );
    typeItem->setDragEnabled( false );
    typeItem->setRenameEnabled( 0, false );
    typeItem = factory_.CreateItem( teamItem );
    typeItem->Set( &LogisticLevel::logistic_base_, tools::translate( "gui::LogisticListView", "Supported units" ) );
    typeItem->setDragEnabled( false );
    typeItem->setRenameEnabled( 0, false );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyDeleted
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::NotifyDeleted( const Team_ABC& team )
{
    delete FindSibling( static_cast< const Entity_ABC* >( &team ), firstChild() );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyDeletedInternal
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::NotifyDeletedInternal( const Entity_ABC& entity )
{
    ValuedListItem* item = FindItem( &entity, firstChild() );
    if( !item )
        return;
    ValuedListItem* childItem = static_cast< ValuedListItem* >( item->firstChild() );
    while( childItem )
    {
        ValuedListItem* nextItem = static_cast< ValuedListItem*>( childItem->nextSibling() );
        const Entity_ABC& child = *childItem->GetValue< const Entity_ABC >();
        SetSuperior( child, 0 );
        childItem = nextItem;
    }
    delete item;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::dragMoveEvent
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void LogisticListView::dragMoveEvent( QDragMoveEvent* pEvent )
{
    const Entity_ABC* entity = gui::ValuedDragObject::GetValue< Entity_ABC >( pEvent );
    if( !entity )
    {
        pEvent->ignore();
        return;
    }
    QPoint position = viewport()->mapFromParent( pEvent->pos() );
    pEvent->accept( CanDrop( entity, position ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::Drop
// Created: ABR 2011-09-15
// -----------------------------------------------------------------------------
bool LogisticListView::Drop( const Entity_ABC& entity, ValuedListItem& target )
{
    bool result = false;
    if( target.IsA< const Entity_ABC >() )
        result = Drop( entity, *target.GetValue< const Entity_ABC >() );
    if( target.IsA< const LogisticLevel >() )
        result = Drop( entity, *target.GetValue< const LogisticLevel >() );
    target.Select( controllers_.actions_ );
    ValuedListItem* item = FindItem( &entity, firstChild() );
    item->Select( controllers_.actions_ );
    return result;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::Drop
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
bool LogisticListView::Drop( const Entity_ABC& source, const Entity_ABC& target )
{
    droppedUnit_ = 0;
    if( ( source.GetTypeName() != Formation_ABC::typeName_ && source.GetTypeName() != Automat_ABC::typeName_ ) ||
        ( target.GetTypeName() != Formation_ABC::typeName_ && target.GetTypeName() != Automat_ABC::typeName_ ) ||
        GetLogisticLevel( target ) == LogisticLevel::none_ )
        return false;
    droppedUnit_ = &source;
    SetSuperior( source, &target );
    return true;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::Drop
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
bool LogisticListView::Drop( const Entity_ABC& source, const LogisticLevel& /*target*/ )
{
    if( RetrieveSuperior( source ) )
    {
        droppedUnit_ = &source;
        SetSuperior( source, 0 );
        return true;
    }
    droppedUnit_ = 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::CanDrop
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
bool LogisticListView::CanDrop( const Entity_ABC* srcEntity, QPoint dstPosition ) const
{
    ValuedListItem* dstItem = static_cast< ValuedListItem* >( itemAt( dstPosition ) );
    if( !srcEntity || !dstItem || !profile_.CanDoMagic( *srcEntity ) )
        return false; // can drop only if we can do magic

    const Formation_ABC* srcFormation = dynamic_cast< const Formation_ABC* >( srcEntity );
    const Automat_ABC* srcAutomat = dynamic_cast< const Automat_ABC* >( srcEntity );
    if( !srcFormation && !srcAutomat )
        return false; // can drop only automats or formations

    const Entity_ABC*    dstEntity =   ( dstItem->IsA< const Entity_ABC >() )    ? dynamic_cast< const Entity_ABC* >( dstItem->GetValue< const Entity_ABC >() )       : 0;
    const LogisticLevel* dstLogistic = ( dstItem->IsA< const LogisticLevel >() ) ? dynamic_cast< const LogisticLevel* >( dstItem->GetValue< const LogisticLevel >() ) : 0;
    if( !dstEntity && !dstLogistic )
        return false; // can only drop to an Entity or a LogisticLevel

    const LogisticLevel& srcLogLevel = GetLogisticLevel( *srcEntity );
    const Entity_ABC* dstTeam = 0;
    if( dstEntity )
    {
        if( dstEntity->GetTypeName() != Automat_ABC::typeName_ && dstEntity->GetTypeName() != Formation_ABC::typeName_ )
            return false; // can drop only to an Automat or a Formation
        if( const TacticalHierarchies* dstHierarchies = dstEntity->Retrieve< TacticalHierarchies >() )
            dstTeam = &dstHierarchies->GetTop();
        const LogisticLevel& dstLogLevel = GetLogisticLevel( *dstEntity );
        if( dstLogLevel == LogisticLevel::none_ )
            return false; // can only drop to an automat / formation which is a logistic base
        if( dstEntity == srcEntity )
            return false; // cannot drop to itself
    }
    else
    {
        assert( dstLogistic );
        ValuedListItem* dstParentItem = static_cast< ValuedListItem* >( dstItem->parent() );
        dstTeam = dynamic_cast< const Entity_ABC* >( dstParentItem->GetValue< const Entity_ABC >() );
        if( srcLogLevel != *dstLogistic )
            return false; // cannot drop logistic base to unsupported item, and vice & versa
    }
    const Entity_ABC* srcTeam = 0;
    if( const TacticalHierarchies* srcHierarchies = srcEntity->Retrieve< TacticalHierarchies >() )
        srcTeam = &srcHierarchies->GetTop();
    assert( srcTeam && dstTeam );
    if( srcTeam != dstTeam )
        return false; // can only drop to the same team

    if( ValuedListItem* srcItem = FindItem( srcEntity, firstChild() ) )
        if( ValuedListItem* srcParentItem = static_cast< ValuedListItem* >( srcItem->parent() ) )
            if( dstItem == srcParentItem )
                return false; // cannot drop on item on his parent
    return true;
}

