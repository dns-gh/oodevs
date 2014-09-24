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

Q_DECLARE_METATYPE( const kernel::Entity_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyTable::LogisticsRequestsSupplyTable( const QString& objectName, QWidget* parent )
    : gui::RichTableView( objectName, parent )
{
    dataModel_.setHorizontalHeaderLabels( QStringList() << tr( "Recipient" )
                                                        << tr( "Supplies" )
                                                        << tr( "Requested" )
                                                        << tr( "Granted" )
                                                        << tr( "Conveyed" )
                                                        << tr( "Delivered" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
    verticalHeader()->setDefaultSectionSize( 22 );

    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 0, linkItemDelegate_ );

    setSortingEnabled( true );
    setFocusPolicy( Qt::NoFocus );
    setSelectionMode( NoSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
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
// Name: LogisticsRequestsSupplyTable::AddRecipientResource
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::AddRecipientResource( const kernel::Entity_ABC& entity,
                                                         const QString& recipient,
                                                         const QString& resource,
                                                         unsigned int requested,
                                                         unsigned int granted,
                                                         unsigned int conveyed,
                                                         bool delivered )
{
    int rowIndex = dataModel_.rowCount();
    auto item = SetData( rowIndex, 0, recipient );
    item->setData( QVariant::fromValue( &entity ), Qt::UserRole );
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
QStandardItem* LogisticsRequestsSupplyTable::SetData( int row, int col, QVariant text, bool checkable, bool checked )
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
        item->setCheckState( checked ? Qt::Checked : Qt::Unchecked );
    dataModel_.setItem( row, col, item );
    return item;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyTable::UpdateRecipient
// Created: ABR 2014-09-22
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyTable::UpdateRecipient( const kernel::Entity_ABC& entity, const QString& name )
{
    for( int i = 0; i < dataModel_.rowCount(); ++i )
        if( auto item = dataModel_.item( i, 0 ) )
            if( auto itemEntity = item->data( Qt::UserRole ).value< const kernel::Entity_ABC* >() )
                if( itemEntity == &entity )
                    item->setText( name );
}
