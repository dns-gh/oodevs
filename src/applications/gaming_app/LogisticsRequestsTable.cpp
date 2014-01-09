// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsTable.h"
#include "moc_LogisticsRequestsTable.cpp"
#include "clients_gui/LinkItemDelegate.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsTable::LogisticsRequestsTable( const QString& objectName, QWidget* parent, const QStringList& horizontalHeaders )
    : gui::RichWidget< QTableView >( objectName, parent )
    , dataModel_ ( parent )
    , proxyModel_( new QSortFilterProxyModel( parent ) )
    , delegate_  ( parent )
    , horizontalHeaders_( horizontalHeaders )
{
    if( horizontalHeaders_.isEmpty() )
    {
        horizontalHeaders_ << tools::translate( "LogisticsRequestsTable", "Id" )
            << tools::translate( "LogisticsRequestsTable", "Requester" )
            << tools::translate( "LogisticsRequestsTable", "Handler" )
            << tools::translate( "LogisticsRequestsTable", "State" );
    }

    dataModel_.setColumnCount( horizontalHeaders.size() );
    proxyModel_->setSourceModel( &dataModel_ );
    proxyModel_->setDynamicSortFilter( true );
    setModel( proxyModel_ );
    sortByColumn( 0, Qt::DescendingOrder );
    setItemDelegate( &delegate_ );

    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 1, linkItemDelegate_ );
    setItemDelegateForColumn( 2, linkItemDelegate_ );

    setSortingEnabled( true );
    setShowGrid( true );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
    verticalHeader()->setDefaultSectionSize( 22 );

    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable destructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsTable::~LogisticsRequestsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::GetLinkItemDelegate
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const gui::LinkItemDelegate* LogisticsRequestsTable::GetLinkItemDelegate() const
{ 
    return linkItemDelegate_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::Purge
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::Purge()
{
    dataModel_.clear();
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders_ );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SelectRequest
// Created: LGY 2014-01-08
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SelectRequest( unsigned int id )
{
    if( dataModel_.rowCount() == 0 || !model() )
        return;

    QModelIndex selected;
    for( int i = 0; i < model()->rowCount(); ++i )
    {
        QModelIndex index = model()->index( i, 0 );
        const LogisticsConsign_ABC* pRequest = GetRequest( index );
        if( pRequest && pRequest->GetId() == id )
        {
            selected = index;
            break;
        }
    }
    selectionModel()->setCurrentIndex( selected, QItemSelectionModel::SelectCurrent| QItemSelectionModel::Rows );
    setFocus();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::GetRequestRow
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
int LogisticsRequestsTable::GetRequestRow( const LogisticsConsign_ABC& consign )
{
    QModelIndexList matches = dataModel_.match( dataModel_.index( 0, 0 ), Qt::UserRole, QVariant::fromValue( &consign )
                                              , 1, Qt::MatchFlags( Qt::MatchWrap | Qt::MatchExactly ) );
    foreach( const QModelIndex &index, matches )
    {
        QStandardItem *item = dataModel_.item( index.row(), index.column() );
        if( item )
            return index.row();
    }
    return dataModel_.rowCount();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::AddRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::AddRequest( const LogisticsConsign_ABC& consign, const QString& id, 
                                         const QString& requester, const QString& handler, const QString& state )
{
    int rowIndex = GetRequestRow( consign );
    SetData( rowIndex, 0, id , consign );
    SetData( rowIndex, 1, requester , consign );
    SetData( rowIndex, 2, handler , consign );
    SetData( rowIndex, 3, state , consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SetData( int row, int col, QString text, const LogisticsConsign_ABC& consign )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( !item )
        item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    item->setData( QVariant::fromValue( &consign ), Qt::UserRole );
    item->setData( QVariant( text ), Qt::DisplayRole );
    item->setTextAlignment( Qt::AlignCenter );
    dataModel_.setItem( row, col, item );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::GetRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const LogisticsConsign_ABC* LogisticsRequestsTable::GetRequest( const QModelIndex& index ) const
{
    if( model() )
        return model()->data( index, Qt::UserRole ).value< const LogisticsConsign_ABC* >();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::GetCurrentRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const LogisticsConsign_ABC* LogisticsRequestsTable::GetCurrentRequest() const
{
    if( selectionModel() && selectionModel()->currentIndex().isValid() )
        return GetRequest( selectionModel()->currentIndex() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::FindRequestsIds
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::FindRequestsIds( std::set< unsigned int >& requests )
{
    for( int i = 0; i < model()->rowCount(); ++i )
    {
        const LogisticsConsign_ABC* pRequest = GetRequest( model()->index( i, 0 ) );
        if( pRequest )
            requests.insert( pRequest->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::ResizeColumns
// Created: LGY 2014-01-06
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::ResizeColumns()
{
    resizeColumnsToContents();
}
