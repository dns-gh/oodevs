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
#include "clients_gui/InternalLinks.h"
#include "clients_gui/Roles.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogisticsConsign_ABC.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable constructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsTable::LogisticsRequestsTable( const QString& objectName,
                                                const QStringList& horizontalHeaders,
                                                const kernel::Controllers& controllers,
                                                const kernel::Profile_ABC& profile,
                                                QWidget* parent )
    : gui::RichTableView( objectName, parent )
    , controllers_( controllers )
    , profile_( profile )
{
    dataModel_.setHorizontalHeaderLabels( !horizontalHeaders.isEmpty() 
        ? horizontalHeaders
        : QStringList() << tr( "Id" )
                        << tr( "Requester" )
                        << tr( "Handler" )
                        << tr( "State" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
    verticalHeader()->setDefaultSectionSize( 22 );
    sortByColumn( 0, Qt::DescendingOrder );

    linkItemDelegate_ = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 3, linkItemDelegate_ );
    connect( linkItemDelegate_, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                              , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );

    //setFocusPolicy( Qt::NoFocus );
    setSortingEnabled( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
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

namespace
{
    QString CreateLink( const QString& message, unsigned long id )
    {
        return gui::InternalLinks::CreateLink( "request", id, message );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::OnLinkClicked
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::OnLinkClicked( const QString&, const QModelIndex& index )
{
    if( const LogisticsConsign_ABC* pRequest = GetRequest( index ) )
        emit RequestSelectionDialog( *pRequest );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::AddRequest
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::AddRequest( const LogisticsConsign_ABC& consign, unsigned int id,
                                         const QString& requester, const QString& handler, const QString& state )
{
    auto base = logistic_helpers::GetLogisticBase( consign.GetHandler() );
    int rowIndex = GetRequestRow( consign );
    SetData( rowIndex, 0, QString::number( id ), id, consign );
    SetData( rowIndex, 1, requester, requester, consign );
    SetData( rowIndex, 2, handler, handler, consign );
    SetData( rowIndex, 3, 
        consign.NeedResolution()
        && base
        && ( profile_.CanBeOrdered( *base ) || tools::CanOneSubordinateBeOrdered( profile_, *base ) )
        && controllers_.GetCurrentMode() != eModes_Replay 
            ? CreateLink( state, consign.GetId() ) : state,
        state, consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SetData( int row, int col, QString displayText,
                                      QVariant sortText, const LogisticsConsign_ABC& consign )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( !item )
        item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    item->setData( QVariant::fromValue( &consign ), Qt::UserRole );
    item->setData( QVariant( displayText ), Qt::DisplayRole );
    item->setData( sortText, gui::Roles::SortRole );
    item->setTextAlignment( Qt::AlignCenter );
    dataModel_.setItem( row, col, item );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetData
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SetData( int col, QString text, const LogisticsConsign_ABC& consign )
{
    int rowIndex = GetRequestRow( consign );
    QStandardItem* item = dataModel_.item( rowIndex, col );
    if( item )
    {
        item->setData( QVariant( text ), Qt::DisplayRole );
        resizeColumnToContents( col );
    }
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
        if( const LogisticsConsign_ABC* pRequest = GetRequest( model()->index( i, 0 ) ) )
            requests.insert( pRequest->GetId() );
}
