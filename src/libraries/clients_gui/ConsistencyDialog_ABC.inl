// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel/ConsistencyChecker_ABC.h"
#include "FilterProxyModel.h"
#include "RichWidget.h"
#include "RichCheckBox.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
ConsistencyDialog< EnumError, TargetItem >::ConsistencyDialog( const QString& objectName, QWidget* parent, kernel::ConsistencyChecker_ABC& checker, FilterProxyModel< EnumError >& proxyModel )
    : ConsistencyDialog_ABC( objectName, parent, checker )
    , proxyModel_( proxyModel )
{
    // Proxy
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.setSourceModel( dataModel_ );
    proxyModel_.setSortRole( Qt::UserRole + 2 );
    proxyModel_.sort( 0, Qt::DescendingOrder );
    tableView_->setModel( &proxyModel_ );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog destructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
ConsistencyDialog< EnumError, TargetItem >::~ConsistencyDialog()
{
    delete &proxyModel_;
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog::AddIcon
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
void ConsistencyDialog< EnumError, TargetItem >::AddIcon( const TargetItem& targetItem, EnumError type, QList< QStandardItem* >& items )
{
    bool isError = IsError( type );
    QStandardItem* item = new QStandardItem( qApp->style()->standardIcon( isError ? QStyle::SP_MessageBoxCritical : QStyle::SP_MessageBoxWarning ), "" );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QVariant variant;
    variant.setValue( kernel::VariantPointer( &targetItem ) );
    item->setData( variant, Qt::UserRole );
    item->setData( QVariant( type ), Qt::UserRole + 1 );
    item->setData( isError, Qt::UserRole + 2 );
    items.push_back( item );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog::AddItem
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
template< typename T >
void ConsistencyDialog< EnumError, TargetItem >::AddItem( T data, QString text, const TargetItem& targetItem, EnumError type, QList< QStandardItem* >& items )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QVariant variant;
    variant.setValue( kernel::VariantPointer( &targetItem ) );
    item->setData( variant, Qt::UserRole );
    item->setData( QVariant( type ), Qt::UserRole + 1 );
    item->setData( data, Qt::UserRole + 2 );
    item->setToolTip( text );
    items.push_back( item );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog::OnFilterChanged
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
void ConsistencyDialog< EnumError, TargetItem >::OnFilterChanged( int type )
{
    proxyModel_.ToggleFilter( static_cast< EnumError >( type ) );
    proxyModel_.setSourceModel( dataModel_ );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog::OnLevelChanged
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename TargetItem >
void ConsistencyDialog< EnumError, TargetItem >::OnLevelChanged()
{
    proxyModel_.SetLevelFilter( warningCheckBox_->isChecked(), errorCheckBox_->isChecked() );
    proxyModel_.setSourceModel( dataModel_ );
}

} //! namespace gui
