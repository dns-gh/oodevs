// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HierarchyTreeView_ABC.h"
#include "moc_HierarchyTreeView_ABC.cpp"
#include "EntitySymbols.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
HierarchyTreeView_ABC::HierarchyTreeView_ABC( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( controllers, profile, modelObserver, parent )
    , symbols_( symbols )
    , timer_( new QTimer( this ) )
{
    timer_->setSingleShot( true );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( OnTimeOut() ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
HierarchyTreeView_ABC::~HierarchyTreeView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::InternalNotifyCreated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::InternalNotifyCreated( const kernel::Hierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    const kernel::Entity_ABC* superior = hierarchy.GetSuperior();
    QStandardItem* entityItem = 0;

    if( superior )      // Child item
    {
        QStandardItem* superiorItem = dataModel_.FindSafeItem( *superior );
        if( !superiorItem )
            superiorItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, superior->GetName(), *superior, ItemSpecificFlags( *superior ) );
        entityItem = dataModel_.AddChildSafeItem( superiorItem, superiorItem->rowCount(), 0, entity.GetName(), entity, ItemSpecificFlags( entity ) );
    }
    else                // Root item
    {
        entityItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, entity.GetName(), entity, ItemSpecificFlags( entity ) );
    }

    if( entityItem)
    {
        UpdateBackgroundColor( *entityItem, entity );
        UpdateSymbol( *entityItem, entity );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::InternalNotifyUpdated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::InternalNotifyUpdated( const kernel::Hierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    QStandardItem* entityItem = dataModel_.FindSafeItem( entity );
    if( entityItem )
    {
        UpdateBackgroundColor( *entityItem, entity );
        UpdateSymbol( *entityItem, entity );

        // Update superior if needed
        const kernel::Entity_ABC* newSuperior = hierarchy.GetSuperior();
        if( newSuperior )
        {
            QStandardItem* newSuperiorItem = dataModel_.FindSafeItem( *newSuperior );
            if( newSuperiorItem )
            {
                QStandardItem* currentSuperiorItem = entityItem->parent();
                if( currentSuperiorItem && currentSuperiorItem != newSuperiorItem )
                {
                    QList< QStandardItem* > rowItems = currentSuperiorItem->takeRow( entityItem->row() );
                    newSuperiorItem->appendRow( rowItems );
                }
            }
            else
                throw std::exception( __FUNCTION__ "Error, superior not found." );
        }
    }
    else
        throw std::exception( __FUNCTION__ "Error, entity not found." );
}

void HierarchyTreeView_ABC::UpdateBackgroundColor( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    // TODO Move following to preparation or gui reimplementation, no ghost on gaming ... or not, all color could also be handle here (log missing, tacticaly destroyed, etc.), options are permissive for that, and it anticipate a gaming/prepa merge
    QColor color = Qt::transparent;
    if( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) != 0 )
        color = QColor( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
    entityItem.setBackground( QBrush( color ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::UpdateSymbol
// Created: JSR 2012-08-30
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::UpdateSymbol( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    QPixmap pixmap = symbols_.GetSymbol( entity );
    if( pixmap.isNull() )
    {
        if( !timer_->isActive() )
            timer_->start( 500 );
        waitingSymbols_.insert( &entity );
        return;
    }
    entityItem.setData( pixmap, Qt::DecorationRole );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::ItemSpecificFlags
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
Qt::ItemFlags HierarchyTreeView_ABC::ItemSpecificFlags( const kernel::Entity_ABC& /*entity*/ ) const
{
    return Qt::ItemIsEditable;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::OnTimeOut
// Created: JSR 2012-08-30
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::OnTimeOut()
{
    std::set< const kernel::Entity_ABC* > localWaitingSymbols;
    std::swap( localWaitingSymbols, waitingSymbols_);
    for( std::set< const kernel::Entity_ABC* >::const_iterator it = localWaitingSymbols.begin(); it != localWaitingSymbols.end(); ++it )
    {
        QStandardItem* entityItem = dataModel_.FindSafeItem( **it );
        if( entityItem )
            UpdateSymbol( *entityItem, **it );
    }
}
