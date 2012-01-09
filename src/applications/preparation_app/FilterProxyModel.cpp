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
FilterProxyModel::FilterProxyModel( QWidget* parent, bool ( *IsError )( E_ConsistencyCheck ) )
    : QSortFilterProxyModel ( parent )
    , IsError_( IsError )
    , warning_( true )
    , error_  ( true )
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
void FilterProxyModel::ToggleFilter( E_ConsistencyCheck type )
{
    if( filters_.find( type ) != filters_.end() )
        filters_.erase( type );
    else
        filters_.insert( type );
    beginResetModel();
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::SetLevelFilter
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
void FilterProxyModel::SetLevelFilter( bool warning, bool error )
{
    warning_ = warning;
    error_ = error;
    beginResetModel();
}

namespace
{
    #define CONVERT_TO_MASK( mask ) { if( type & mask ) return mask; }

    E_ConsistencyCheck Convert( E_ConsistencyCheck type )
    {
        CONVERT_TO_MASK( eUniquenessMask )
        CONVERT_TO_MASK( eLogisticMask )
        CONVERT_TO_MASK( eProfileMask )
        CONVERT_TO_MASK( eGhostMask )
        CONVERT_TO_MASK( eCommandPostMask )
        return eOthersMask;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::filterAcceptsRow
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
bool FilterProxyModel::filterAcceptsRow( int row, const QModelIndex& parent ) const
{
    QModelIndex index = sourceModel()->index( row, 0, parent );
    E_ConsistencyCheck type = static_cast< E_ConsistencyCheck >( index.data( Qt::UserRole + 1 ).toInt() );
    if( filters_.find( Convert( type ) ) == filters_.end() )
        return false;
    bool isError = IsError_( type );
    return ( warning_ && !isError ) || ( error_ && isError );
}
