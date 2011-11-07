// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterProxyModel.h"
#include "moc_FilterProxyModel.cpp"

// -----------------------------------------------------------------------------
// Name: FilterProxyModel constructor
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
FilterProxyModel::FilterProxyModel( QWidget* parent )
    : QSortFilterProxyModel ( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel destructor
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
FilterProxyModel::~FilterProxyModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::ToggleFilter
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
void FilterProxyModel::ToggleFilter( ModelConsistencyChecker::E_ConsistencyCheck type )
{
    if( filters_.find( type ) != filters_.end() )
        filters_.erase( type );
    else
        filters_.insert( type );
    beginResetModel();
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::filterAcceptsRow
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
bool FilterProxyModel::filterAcceptsRow( int row, const QModelIndex& parent ) const
{
    QModelIndex index = sourceModel()->index( row, 0, parent );
    ModelConsistencyChecker::E_ConsistencyCheck type = static_cast< ModelConsistencyChecker::E_ConsistencyCheck >( index.data( Qt::UserRole + 1 ).toInt() );
    if( filters_.find( type ) != filters_.end() )
        return true;
    return false;
}
