// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTable_ABC.h"
#include "moc_UnitStateTable_ABC.cpp"

#include "clients_gui/Roles.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::UnitStateTable_ABC( const QString& objectName,
                                        QWidget* parent,
                                        kernel::Controllers& controllers,
                                        const QStringList& horizontalHeaders )
    : RichTableView( objectName, parent )
    , aggregated_( false )
    , selected_  ( controllers )
{
    dataModel_.setColumnCount( horizontalHeaders.size() );
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setSortRole( Qt::UserRole );
    proxyModel_.setFilterRole( Roles::FilterRole );
    proxyModel_.setFilterRegExp( StandardModel::showValue_ );

    setModel( &proxyModel_ );
    setItemDelegate( &delegate_ );

    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );

    setSortingEnabled( true );
    setShowGrid( false );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
    setSelectionMode( NoSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::~UnitStateTable_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::Purge
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::Purge()
{
    RichTableView::Purge();
    aggregated_ = false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::RecursiveLoad
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::RecursiveLoad( kernel::Entity_ABC& entity, bool isSelectedEntity )
{
    InternalRecursiveLoad( entity, isSelectedEntity );
    emit RefreshFilters();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::InternalRecursiveLoad
// Created: ABR 2014-05-15
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::InternalRecursiveLoad( kernel::Entity_ABC& entity, bool isSelectedEntity )
{
    if( isSelectedEntity )
        selected_ = &entity;
    const std::string& typeName = entity.GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ )
        Load( entity );
    else
    {
        aggregated_ = true;
        assert( typeName == kernel::Automat_ABC::typeName_ || typeName == kernel::Formation_ABC::typeName_ || typeName == kernel::Team_ABC::typeName_ );
        const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
            RecursiveLoad( const_cast< kernel::Entity_ABC& >( it.NextElement() ), false );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::SetColor
// Created: ABR 2014-01-31
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::SetColor( int row, int col, const QBrush& brush, int colorIndex )
{
    if( QStandardItem* item = dataModel_.item( row, col ) )
    {
        item->setBackground( brush );
        item->setData( colorIndex >= 0 ? static_cast< unsigned int >( colorIndex ) : QVariant(), gui::Roles::ExcelColorRole );
    }
}
