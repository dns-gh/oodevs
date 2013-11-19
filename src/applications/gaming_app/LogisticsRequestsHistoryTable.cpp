// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsHistoryTable.h"
#include "moc_LogisticsRequestsHistoryTable.cpp"
#include "clients_gui/LinkItemDelegate.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsHistoryTable::LogisticsRequestsHistoryTable( const QString& objectName, QWidget* parent )
    : gui::RichWidget< QTableView >( objectName, parent )
{
    horizontalHeaders_ << tools::translate( "LogisticsRequestsHistoryTable", "Previous state" )
        << tools::translate( "LogisticsRequestsHistoryTable", "Started" )
        << tools::translate( "LogisticsRequestsHistoryTable", "Ended" )
        << tools::translate( "LogisticsRequestsHistoryTable", "Handler" );

    dataModel_          = new QStandardItemModel( parent );
    proxyModel_         = new QSortFilterProxyModel( parent );
    delegate_           = new gui::CommonDelegate( parent );
    linkItemDelegate_   = new gui::LinkItemDelegate( this );

    dataModel_->setColumnCount( horizontalHeaders_.size() );
    proxyModel_->setSourceModel( dataModel_ );
    proxyModel_->setSortRole( Qt::UserRole );
    setModel( proxyModel_ );
    setItemDelegate( delegate_ );
    setItemDelegateForColumn( 3, linkItemDelegate_ );

    setSortingEnabled( true );
    setShowGrid( true );
    setEnabled( true );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
    setFocusPolicy( Qt::NoFocus );

    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable destructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsHistoryTable::~LogisticsRequestsHistoryTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::GetLinkItemDelegate
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const gui::LinkItemDelegate* LogisticsRequestsHistoryTable::GetLinkItemDelegate() const
{ 
    return linkItemDelegate_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::Purge
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsHistoryTable::Purge()
{
    dataModel_->clear();
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( dataModel_->columnCount() - 1, QHeaderView::Stretch );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::AddRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsHistoryTable::AddRequest( const QString& state, const QString& started, const QString& ended, const QString& handler )
{
    int rowIndex = dataModel_->rowCount();
    AddItem( rowIndex, 0, state );
    AddItem( rowIndex, 1, started );
    AddItem( rowIndex, 2, ended );
    AddItem( rowIndex, 3, handler );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::AddItem
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsHistoryTable::AddItem( int row, int col, QString text )
{
    QStandardItem* item = new QStandardItem( text );
    if( col == 1 || col == 2 )
        item->setTextAlignment( Qt::AlignCenter );
    else
        item->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    dataModel_->setItem( row, col, item );
}
