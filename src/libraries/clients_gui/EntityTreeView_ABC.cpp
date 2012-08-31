// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityTreeView_ABC.h"
#include "moc_EntityTreeView_ABC.cpp"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC constructor
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
EntityTreeView_ABC::EntityTreeView_ABC( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, QWidget* parent /*= 0*/ )
    : RichTreeView( controllers, parent )
    , controllers_( controllers )
    , profile_    ( profile )
    , blockSelect_( false )
{
    setHeaderHidden( true );

    connect( this,             SIGNAL( activated       ( const QModelIndex& ) ),                           this, SLOT( OnActivate( const QModelIndex& ) ) );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelect  ( const QItemSelection&, const QItemSelection& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC destructor
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
EntityTreeView_ABC::~EntityTreeView_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyActivated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyActivated( const kernel::Entity_ABC& entity )
{
    if( IsTypeRejected( entity ) )
        return;
    QStandardItem* item = dataModel_.FindSafeItem( entity );
    if( item )
        scrollTo( item->index() );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnActivate
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnActivate( const QModelIndex& index )
{
    kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index );
    if( entity )
        entity->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyCreated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyCreated( const kernel::Team_ABC& team )
{
    const kernel::Entity_ABC& entityTeam = static_cast< const kernel::Entity_ABC& >( team );
    QStandardItem* teamItem = dataModel_.FindSafeItem( entityTeam );
    if( !teamItem )
    {
        teamItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, team.GetName(), entityTeam );
        //teamItem->SetNamed( team );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyDeleted
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyDeleted( const kernel::Team_ABC& /* team */ )
{
    dataModel_.PurgeObsoleteSafeItem< kernel::Entity_ABC >();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyUpdated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
//void EntityTreeView_ABC::NotifyUpdated( const kernel::Entity_ABC& entity )
//{
//    SetNamed()
//}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::ApplyProfileFilter
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::ApplyProfileFilter( QStandardItem* item ) const
{
    if( !item )
        return false;
    kernel::Entity_ABC* entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( *item );
    if( entity )
        return profile_.IsVisible( *entity );
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyUpdated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyUpdated( const kernel::Profile_ABC& /* profile */ )
{
    dataModel_.ApplyFilter( boost::bind( &EntityTreeView_ABC::ApplyProfileFilter, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifySelectionChanged
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    blockSelect_ = true;
    selectionModel()->clearSelection();
    for( std::vector< const kernel::Entity_ABC* >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        if( *it && !IsTypeRejected( **it ) )
            if( QStandardItem* item = dataModel_.FindSafeItem( **it ) )
            {
                selectionModel()->select( item->index(), QItemSelectionModel::Select );
                scrollTo( item->index() );
            }
    }
    blockSelect_ = false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnSelect
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnSelect( const QItemSelection& selected, const QItemSelection& /* deselected */ )
{
    if( blockSelect_ )
        return;

    QModelIndexList selectedIndexes = selected.indexes();
    kernel::ActionController::T_Selectables list;
    for( QModelIndexList::const_iterator it = selectedIndexes.constBegin(); it != selectedIndexes.constEnd(); ++it )
    {
        kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( *it );
        if( entity )
            list.push_back( entity );
    }
    controllers_.actions_.SetMultipleSelection( list );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::contextMenuEvent
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::contextMenuEvent( QContextMenuEvent* event )
{
    if( !IsReadOnly() && event )
    {
     //   kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( indexAt( event->globalPos() ) );
        //if( ValuedListItem* value = dynamic_cast< ValuedListItem* >( i ) )
        //    value->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::IsTypeRejected
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return false;
}
