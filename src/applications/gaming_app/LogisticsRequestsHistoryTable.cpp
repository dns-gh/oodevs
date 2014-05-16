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
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

namespace
{
    enum EColumns
    {
        eColumnState,
        eColumnStarted,
        eColumnEnded,
        eColumnHandler
    };
}
// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsHistoryTable::LogisticsRequestsHistoryTable( const QString& objectName, QWidget* parent )
    : gui::RichTableView( objectName, parent )
{
    dataModel_.setHorizontalHeaderLabels( QStringList() << tr( "Previous state" )
                                                         << tr( "Started" )
                                                         << tr( "Ended" )
                                                         << tr( "Handler" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );

    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( eColumnHandler, linkItemDelegate_ );

    setSortingEnabled( false );
    setFocusPolicy( Qt::NoFocus );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
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
// Name: LogisticsRequestsHistoryTable::ItemHasText
// Created: JSR 2014-02-25
// -----------------------------------------------------------------------------
bool LogisticsRequestsHistoryTable::ItemHasText( int row, int col, const QString& text ) const
{
     QStandardItem* item = dataModel_.item( row, col );
     return item && item->text() == text;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::AddRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsHistoryTable::AddRequest( const QString& state, const QString& started, const QString& ended, const QString& handler )
{
    const int rowIndex = dataModel_.rowCount();
    for( int row = 0; row < rowIndex; ++row )
    {
        if( ItemHasText( row, eColumnState, state ) && ItemHasText( row, eColumnHandler, handler ) )
        {
            if( ItemHasText( row, eColumnStarted, ended ) )
            {
                dataModel_.item( row, eColumnStarted )->setText( started );
                return;
            }
            if( ItemHasText( row, eColumnEnded, started ) )
            {
                dataModel_.item( row, eColumnEnded )->setText( ended );
                return;
            }
        }
    }
    AddItem( rowIndex, eColumnState, state );
    AddItem( rowIndex, eColumnStarted, started );
    AddItem( rowIndex, eColumnEnded, ended );
    AddItem( rowIndex, eColumnHandler, handler );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsHistoryTable::AddItem
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsHistoryTable::AddItem( int row, int col, const QString& text )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    if( col == eColumnStarted || col == eColumnEnded )
        item->setTextAlignment( Qt::AlignCenter );
    else
        item->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    dataModel_.setItem( row, col, item );
}
