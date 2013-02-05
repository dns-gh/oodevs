// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogisticTreeView.h"
#include "LongNameHelper.h"
#include "DragAndDropHelpers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"

using namespace gui;

namespace
{
    int GetDepth( const QModelIndex& index )
    {
        int depth = 0;
        QModelIndex p = index;
        while( p.isValid() )
        {
            p = p.parent();
            ++depth;
        }
        return depth;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView constructor
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
LogisticTreeView::LogisticTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : HierarchyTreeView_ABC( controllers, profile, modelObserver, symbols, parent )
{
    controllers_.Update( *this );
    EnableDragAndDrop( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView destructor
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
LogisticTreeView::~LogisticTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::GetDecoration
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
const QPixmap* LogisticTreeView::GetDecoration( const QModelIndex& index )
{
    if( GetDepth( index ) == 2 )
        return 0;
    return HierarchyTreeView_ABC::GetDecoration( index );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::OnActivate
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::OnActivate( const QModelIndex& index )
{
    if( GetDepth( index ) == 2 )
        return;
    HierarchyTreeView_ABC::OnActivate( index );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::GetLogisticLevel
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& LogisticTreeView::GetLogisticLevel( const kernel::Entity_ABC& entity ) const
{
    if( entity.GetTypeName() == kernel::Formation_ABC::typeName_ )
        return static_cast< const kernel::Formation_ABC* >( &entity )->GetLogisticLevel();
    else if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ )
        return static_cast< const kernel::Automat_ABC* >( &entity )->GetLogisticLevel();
    else if( entity.GetTypeName() == kernel::Ghost_ABC::typeName_ )
        return static_cast< const kernel::Ghost_ABC* >( &entity )->GetLogisticLevel();
    assert( false );
    return kernel::LogisticLevel::none_;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::RetrieveTeamItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
QStandardItem& LogisticTreeView::RetrieveTeamItem( const kernel::Entity_ABC& entity )
{
    const kernel::TacticalHierarchies& hierarchies = entity.Get< kernel::TacticalHierarchies >();
    const kernel::Entity_ABC& team = hierarchies.GetTop();
    QStandardItem* teamItem = dataModel_.FindDataItem( team );
    assert( teamItem );
    return *teamItem;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::RetrieveTypeItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
QStandardItem& LogisticTreeView::RetrieveTypeItem( const kernel::Entity_ABC& entity, QStandardItem& teamItem )
{
    const kernel::LogisticLevel& logisticLevel = GetLogisticLevel( entity );
    QStandardItem* typeItem = dataModel_.FindDataItem( logisticLevel, &teamItem );
    assert( typeItem );
    return *typeItem;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyCreatedInternal
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::CreateOrReplace( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( GetLogisticLevel( entity ) == kernel::LogisticLevel::none_ && entity.GetTypeName() == kernel::Formation_ABC::typeName_ )
    {
        if( item )
        {
            dataModel_.PurgeChildren( *item );
            dataModel_.DeleteItemRow( item );
        }
        return;
    }

    QStandardItem& teamItem = RetrieveTeamItem( entity );
    QStandardItem& typeItem = RetrieveTypeItem( entity, teamItem );

    const kernel::Entity_ABC* superior = RetrieveSuperior( entity );
    QStandardItem* superiorItem = ( superior ) ? dataModel_.FindDataItem( *superior ) : 0;

    if( !item )
    {
        QStandardItem* parent = ( superiorItem ) ? superiorItem : &typeItem;
        AddItemIfPossible< kernel::Formation_ABC >( entity, parent, item );
        AddItemIfPossible< kernel::Automat_ABC >( entity, parent, item );
        AddItemIfPossible< kernel::Ghost_ABC >( entity, parent, item );
    }
    else if( item != superiorItem )
    {
        QStandardItem* parentItem = item->parent();
        if( superiorItem && parentItem != superiorItem )
            dataModel_.MoveItem( *item, *superiorItem );
        else if( !superiorItem && parentItem != &typeItem )
            dataModel_.MoveItem( *item, typeItem );
    }
    item->setEditable( !gui::longname::SetItemLongName( entity, *item ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdatedInternal
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void LogisticTreeView::UpdateLongName( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
        item->setEditable( !gui::longname::SetItemLongName( entity, *item ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyDeletedInternal
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyDeletedInternal( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( !item )
        return;

    // $$$$ ABR 2012-09-20: Reparent all children
    for( int row = 0; row < item->rowCount(); ++row )
    {
        QStandardItem* childItem = item->child( row, 0 );
        assert( childItem );
        const kernel::Entity_ABC* child = dataModel_.GetDataFromItem< kernel::Entity_ABC >( *childItem );
        assert( child );
        SetSuperior( *child, 0 );
    }
}

// -----------------------------------------------------------------------------
// Team
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyCreated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyCreated( const kernel::Team_ABC& team )
{
    const kernel::Entity_ABC& entityTeam = static_cast< const kernel::Entity_ABC& >( team );
    QStandardItem* teamItem = dataModel_.FindDataItem( entityTeam );
    if( teamItem )
        return;

    teamItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, team.GetName(), team.GetTooltip(), team );
    QStandardItem* item = dataModel_.AddChildDataItem( teamItem, teamItem->rowCount(), 0, tools::translate( "gui::LogisticTreeView", "Unsupported units" ), "", kernel::LogisticLevel::none_, Qt::ItemIsDropEnabled );
    if( item )
        item->setSelectable( false );
    item = dataModel_.AddChildDataItem( teamItem, teamItem->rowCount(), 0, tools::translate( "gui::LogisticTreeView", "Supported units" ), "", kernel::LogisticLevel::logistic_base_, Qt::ItemIsDropEnabled );
    if( item )
        item->setSelectable( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const kernel::Team_ABC& team )
{
    UpdateLongName( team );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyDeleted
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyDeleted( const kernel::Team_ABC& team )
{
    QStandardItem* teamItem = dataModel_.FindDataItem( team );
    if( teamItem )
        dataModel_.DeleteTree( teamItem );
}

// -----------------------------------------------------------------------------
// Formation
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyCreated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyCreated( const kernel::Formation_ABC& formation )
{
    CreateOrReplace( formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const kernel::Formation_ABC& formation )
{
    UpdateLongName( formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyDeleted
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    NotifyDeletedInternal( formation );
}

// -----------------------------------------------------------------------------
// Automat
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyCreated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyCreated( const kernel::Automat_ABC& automat )
{
    CreateOrReplace( automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const kernel::Automat_ABC& automat )
{
    UpdateLongName( automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyDeleted
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    NotifyDeletedInternal( automat );
}

// -----------------------------------------------------------------------------
// Ghost
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyCreated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyCreated( const kernel::Ghost_ABC& ghost )
{
    if( ghost.GetGhostType() == eGhostType_Automat)
        CreateOrReplace( ghost );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const kernel::Ghost_ABC& ghost )
{
    if( ghost.GetGhostType() == eGhostType_Automat)
        UpdateLongName( ghost );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyDeleted
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyDeleted( const kernel::Ghost_ABC& ghost )
{
    if( ghost.GetGhostType() == eGhostType_Automat)
        NotifyDeletedInternal( ghost );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: NPT 2012-12-13
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const kernel::TacticalHierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = static_cast< const kernel::Hierarchies& >( hierarchy ).GetEntity();
    QStandardItem* entityItem = dataModel_.FindDataItem( entity );
    if( !entityItem )
        return;

    QStandardItem* parent = entityItem->parent();
    while( parent && parent->parent() )
        parent = parent->parent();
    if( !parent )
        return;
    kernel::Team_ABC* oldTeam = dataModel_.GetDataFromItem< kernel::Team_ABC >( *parent );

    if( oldTeam && oldTeam->GetId() != hierarchy.GetTop().GetId() )
    {
        //Faire sauter les liens log et replacer correctement sous la bonne team
        SetSuperior( entity, 0 );
        CreateOrReplace( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::MimeTypes
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
QStringList LogisticTreeView::MimeTypes() const
{
    QStringList l;
    l << typeid( kernel::Automat_ABC ).name() << typeid( kernel::Formation_ABC ).name()
      << typeid( kernel::LogisticLevel ).name();
    return l;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::dragMoveEvent
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
void LogisticTreeView::dragMoveEvent( QDragMoveEvent* pEvent )
{
    QTreeView::dragMoveEvent( pEvent );
    if( CanDrop( pEvent ) )
    {
        pEvent->accept();
        return;
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::CanDrop
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
bool LogisticTreeView::CanDrop( QDragMoveEvent* pEvent ) const
{
    kernel::AutomatType* sourceAutomatType = dnd::FindData< kernel::AutomatType >( pEvent );
    kernel::Entity_ABC*  sourceEntity = dnd::FindSafeEntityData< kernel::Formation_ABC, kernel::Automat_ABC, kernel::Ghost_ABC >( pEvent );

    QStandardItem* sourceItem = ( sourceEntity ) ? dataModel_.FindTextItem( sourceEntity->GetTypeName().c_str() ) : 0;
    QStandardItem* targetItem = dataModel_.GetItemFromIndex( indexAt( pEvent->pos() ) );

    if( IsReadOnly() )
        return false; // can drop only if not read only

    if( ( !sourceAutomatType && !sourceEntity ) || !targetItem )
        return false; // can drop only if we have a source and a target

    if( !targetItem->isDropEnabled() )
        return false;

    kernel::LogisticLevel* targetLogistic  = ( dnd::IsA< kernel::LogisticLevel >( *targetItem ) ) ? dataModel_.GetDataFromItem< kernel::LogisticLevel >( *targetItem ) : 0;
    kernel::Entity_ABC* targetEntity = dnd::FindSafeEntityData< kernel::Formation_ABC, kernel::Automat_ABC, kernel::Ghost_ABC >( dataModel_, *targetItem );

    if( sourceItem && sourceItem->parent() == targetItem )
        return false; // cannot drop an item on his parent

    if( sourceAutomatType && ( ( targetEntity && targetEntity->GetTypeName() != kernel::Ghost_ABC::typeName_ ) || !targetEntity ) )
        return false; // can drop automat type only on a ghost

    if( ( sourceEntity && !profile_.CanDoMagic( *sourceEntity ) ) || ( targetEntity && !profile_.CanDoMagic( *targetEntity ) ) )
        return false; // can drop only if we can do magic


    if( sourceEntity )
    {
        const kernel::LogisticLevel& sourceLogLevel = GetLogisticLevel( *sourceEntity );
        const kernel::Entity_ABC* targetTeam = 0;
        if( targetEntity )
        {
            if( const kernel::TacticalHierarchies* targetHierarchies = targetEntity->Retrieve< kernel::TacticalHierarchies >() )
                targetTeam = &targetHierarchies->GetTop();
            const kernel::LogisticLevel& targetLogLevel = GetLogisticLevel( *targetEntity );
            if( targetLogLevel == kernel::LogisticLevel::none_ )
                return false; // can only drop to an automat / formation which is a logistic base
            if( targetEntity == sourceEntity )
                return false; // cannot drop to itself
        }
        else
        {
            assert( targetLogistic );
            QStandardItem* targetTeamItem = targetItem->parent();
            assert( targetTeamItem );
            targetTeam = dataModel_.GetDataFromItem< kernel::Team_ABC >( *targetTeamItem );
            if( sourceLogLevel != *targetLogistic )
                return false; // cannot drop logistic base to unsupported item, and vice & versa
        }
        const kernel::Entity_ABC* sourceTeam = 0;
        if( const kernel::TacticalHierarchies* sourceHierarchies = sourceEntity->Retrieve< kernel::TacticalHierarchies >() )
            sourceTeam = &sourceHierarchies->GetTop();
        assert( sourceTeam && targetTeam );
        if( sourceTeam != targetTeam )
            return false; // can only drop to the same team
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::Drop
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
void LogisticTreeView::Drop( const QString& mimeType, void* data, QStandardItem& targetItem )
{
    if( IsReadOnly() )
        return;

    kernel::LogisticLevel* targetLogistic  = ( dnd::IsA< kernel::LogisticLevel >( targetItem ) ) ? dataModel_.GetDataFromItem< kernel::LogisticLevel >( targetItem ) : 0;
    kernel::Entity_ABC* targetEntity = dnd::FindSafeEntityData< kernel::Formation_ABC, kernel::Automat_ABC, kernel::Ghost_ABC >( dataModel_, targetItem );

    if( mimeType == typeid( kernel::AutomatType ).name() && targetEntity ) // Drop AutomatType on Ghost
    {
        assert( targetEntity->GetTypeName() == kernel::Ghost_ABC::typeName_ );
        const kernel::AutomatType* type = reinterpret_cast< kernel::AutomatType* >( data );
        if( type )
            HierarchyTreeView_ABC::Drop( *type, *targetEntity );
    }
    else if( mimeType == typeid( kernel::Automat_ABC ).name() ||
             mimeType == typeid( kernel::Formation_ABC ).name() ||
             mimeType == typeid( kernel::Ghost_ABC ).name() )
    {
        kernel::SafePointer< const kernel::Entity_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::Entity_ABC >* >( data );
        if( !safePtr || !*safePtr )
            return;
        if( targetEntity )
            SetSuperior( **safePtr, targetEntity );
        else if( targetLogistic && RetrieveSuperior( **safePtr ) )
            SetSuperior( **safePtr, 0 );

        ( *safePtr )->Select( controllers_.actions_ );

        kernel::Selectable_ABC::T_Selectables list;
        list.push_back( *safePtr );
        ( *safePtr )->MultipleSelect( controllers_.actions_, list );
    }
}
