// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsDetailsTable.h"
#include "moc_LogisticsRequestsDetailsTable.cpp"
#include "clients_gui/LinkItemDelegate.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsDetailsTable::LogisticsRequestsDetailsTable( const QString& objectName, QWidget* parent )
    : gui::RichWidget< QTableView >( objectName, parent )
    , detailIndex_( 0 )
{
    dataModel_ = new QStandardItemModel( this );
    proxyModel_= new QSortFilterProxyModel( this );
    delegate_  = new gui::CommonDelegate( this );
    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    dataModel_->setColumnCount( 4 );
    dataModel_->setRowCount( 4 );
    proxyModel_->setSourceModel( dataModel_ );
    proxyModel_->setSortRole( Qt::UserRole );
    setModel( proxyModel_ );
    setItemDelegate( delegate_ );
    setItemDelegateForColumn( 1, linkItemDelegate_ );
    setItemDelegateForColumn( 3, linkItemDelegate_ );

    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setSortingEnabled( false );
    setShowGrid( false );
    setAlternatingRowColors( false );
    horizontalHeader()->setVisible( false );
    verticalHeader()->setVisible( false );
    setSelectionMode( NoSelection );
    setEditTriggers( NoEditTriggers );
    setFrameStyle( QFrame::NoFrame );
    setFocusPolicy( Qt::NoFocus );
    verticalHeader()->setDefaultSectionSize( 22 );
    setFixedHeight( dataModel_->rowCount() * rowHeight( 0 ) + ( frameWidth() * 2 ) );

    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable destructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsDetailsTable::~LogisticsRequestsDetailsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable::GetLinkItemDelegate
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
const gui::LinkItemDelegate* LogisticsRequestsDetailsTable::GetLinkItemDelegate() const
{ 
    return linkItemDelegate_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable::Purge
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsDetailsTable::Purge()
{
    detailIndex_ = 0;
    int columnCount = dataModel_->columnCount();
    int rowCount = dataModel_->rowCount();
    dataModel_->clear();
    dataModel_->setColumnCount( columnCount );
    dataModel_->setRowCount( rowCount );
    for( int i = 0; i < rowCount; ++i )
        for( int j = 0; j < columnCount; ++j )
        {
            QStandardItem* pItem = new QStandardItem( QString() );
            pItem->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
            pItem->setBackground( QBrush( palette().background().color() ) );
            if( j%2 == 0 )
            {
                QFont curFont = pItem->font();
                curFont.setBold( true );
                curFont.setWeight( curFont.weight() - 2 );
                pItem->setFont( curFont );
            }
            dataModel_->setItem( i, j, pItem );
        }
    for( int i = 0; i < columnCount; ++i )
        horizontalHeader()->setResizeMode( i, ( i < (columnCount - 1 ) )? QHeaderView::ResizeToContents : QHeaderView::Stretch );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsDetailsTable::SetData( int row, int col, QString text )
{
    QStandardItem* item = dataModel_->item( row, col );
    if( item )
        item->setData( QVariant( text ), Qt::DisplayRole );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsDetailsTable::Add
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsDetailsTable::Add( const QString& title, const QString& value )
{
    int curRow = detailIndex_ % dataModel_->rowCount();
    int curCol = ( detailIndex_ / dataModel_->rowCount() ) * 2;
    QStandardItem* itemTitle = dataModel_->item( curRow, curCol );
    QStandardItem* itemValue = dataModel_->item( curRow, curCol + 1 );
    if( itemTitle )
        itemTitle->setData( QVariant( title ), Qt::DisplayRole );
    if( itemValue )
        itemValue->setData( QVariant( value ), Qt::DisplayRole );
    ++detailIndex_;
}
