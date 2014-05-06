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

namespace
{
    class SupplyDelegate : public gui::CommonDelegate
    {
        public:
            SupplyDelegate( QObject *parent = 0 )
                : gui::CommonDelegate( parent )
            {}
        public:
        virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
        {
            Qt::ItemFlags flags = index.flags();
            if( ( flags & Qt::ItemIsUserCheckable ) )
            {
                // Draw background
                painter->save();
                drawBackground(painter, option, index);
                painter->restore();

                // Draw content
                QStyleOptionViewItemV4 viewItemOption( option );
                const int textMargin = QApplication::style()->pixelMetric( QStyle::PM_FocusFrameHMargin ) + 1;
                viewItemOption.decorationAlignment = Qt::AlignCenter;
                QRect newRect = QStyle::alignedRect( option.direction, Qt::AlignCenter,
                                                        QSize( option.decorationSize.width() + 5, option.decorationSize.height() ),
                                                        QRect( option.rect.x(), option.rect.y(), option.rect.width() - ( 2 * textMargin ), option.rect.height() ) );

                // Print original
                viewItemOption.rect = newRect;
                gui::CommonDelegate::paint( painter, viewItemOption, index );
            }
            else
                gui::CommonDelegate::paint( painter, option, index );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyTable::LogisticsRequestsSupplyTable( const QString& objectName, QWidget* parent )
    : gui::RichTableView( objectName, parent )
    , dataModel_ ( parent )
    , proxyModel_( parent )
    , delegate_  ( new SupplyDelegate( parent ) )
{
    horizontalHeaders_ << tools::translate( "LogisticsRequestsSupplyTable", "Recipient" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Supplies" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Requested" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Granted" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Conveyed" )
        << tools::translate( "LogisticsRequestsSupplyTable", "Delivered" );

    dataModel_.setColumnCount( horizontalHeaders_.size() );
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setDynamicSortFilter( true );

    dataModel_.setHorizontalHeaderLabels( horizontalHeaders_ );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );

    setModel( &proxyModel_ );
    setItemDelegate( delegate_ );

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
    dataModel_.removeRows( 0, dataModel_.rowCount() );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable::AddRecipientResource
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::AddRecipientResource( const QString& recipient, const QString& resource,
                                                         unsigned int requested, unsigned int granted,
                                                         unsigned int conveyed, bool delivered )
{
    int rowIndex = dataModel_.rowCount();
    SetData( rowIndex, 0, recipient );
    SetData( rowIndex, 1, resource );
    SetData( rowIndex, 2, requested );
    SetData( rowIndex, 3, granted );
    SetData( rowIndex, 4, conveyed );
    SetData( rowIndex, 5, QVariant(), true, delivered );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::SetData( int row, int col, QVariant text, bool checkable, bool checked )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( !item )
        item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    if( !checkable )
    {
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        item->setData( text, Qt::DisplayRole );
        item->setTextAlignment( Qt::AlignCenter );
    }
    else
    {
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
        item->setCheckState( checked ? Qt::Checked : Qt::Unchecked );
    }
    dataModel_.setItem( row, col, item );
}
