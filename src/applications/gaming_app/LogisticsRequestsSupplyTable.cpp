// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsSupplyTable.h"
#include "moc_LogisticsRequestsSupplyTable.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/LinkItemDelegate.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyTable::LogisticsRequestsSupplyTable( const QString& objectName, QWidget* parent )
    : gui::RichWidget< QTableView >( objectName, parent )
    , dataModel_ ( parent )
    , proxyModel_( parent )
    , delegate_  ( parent )
{
    horizontalHeaders_ << tools::translate( "LogisticsRequestsSupplyTable", "Recipient" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Resource" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Requested" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Granted" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Conveyed" );

    dataModel_.setColumnCount( horizontalHeaders_.size() );
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setSortRole( Qt::UserRole );
    setModel( &proxyModel_ );
    setItemDelegate( &delegate_ );

    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 0, linkItemDelegate_ );

    setSortingEnabled( true );
    setShowGrid( true );
    setFocusPolicy( Qt::NoFocus );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
    verticalHeader()->setDefaultSectionSize( 22 );

    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable destructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyTable::~LogisticsRequestsSupplyTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable::GetLinkItemDelegate
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const gui::LinkItemDelegate* LogisticsRequestsSupplyTable::GetLinkItemDelegate() const
{ 
    return linkItemDelegate_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable::Purge
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::Purge()
{
    dataModel_.clear();
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders_ );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( dataModel_.columnCount() - 1, QHeaderView::Stretch );    
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable::AddRecipientResource
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::AddRecipientResource( const QString& recipient, const QString& resource
                                                       , const QString& requested, const QString& granted, const QString& conveyed )
{
    int rowIndex = dataModel_.rowCount();
    SetData( rowIndex, 0, recipient );
    SetData( rowIndex, 1, resource );
    SetData( rowIndex, 2, requested );
    SetData( rowIndex, 3, granted );
    SetData( rowIndex, 4, conveyed );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::SetData( int row, int col, QString text )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( !item )
        item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    item->setData( QVariant( text ), Qt::DisplayRole );
    item->setTextAlignment( Qt::AlignCenter );
    dataModel_.setItem( row, col, item );
}
