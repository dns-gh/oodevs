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
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Roles.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
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
    requesterNameExtractor_ = []( const LogisticsConsign_ABC& consign ) -> QString {
        auto consumer = consign.GetConsumer();
        return consumer ? consumer->GetName() : "";
    };
    isEntityInConsignFunctor_ = []( const LogisticsConsign_ABC& consign, const kernel::Entity_ABC& entity ) {
        return consign.GetConsumer() == &entity || consign.GetHandler() == &entity;
    };

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
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable destructor
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
LogisticsRequestsTable::~LogisticsRequestsTable()
{
    controllers_.controller_.Unregister( *this );
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
void LogisticsRequestsTable::AddRequest( const LogisticsConsign_ABC& consign )
{
    if( !requesterNameExtractor_ )
        return;
    auto handler = consign.GetHandler();
    auto handlerName = handler ? handler->GetName() : "";
    auto requesterName = requesterNameExtractor_( consign );
    auto state = consign.GetStatusDisplay();
    auto base = logistic_helpers::GetLogisticBase( handler );
    int rowIndex = GetRequestRow( consign );
    auto id = consign.GetId();
    SetData( rowIndex, 0, QString::number( id ), id, consign );
    SetData( rowIndex, 1, requesterName, requesterName, consign );
    SetData( rowIndex, 2, handlerName, handlerName, consign );
    SetData( rowIndex, 3, 
        consign.NeedResolution()
        && base
        && tools::CanOneSubordinateBeOrdered( profile_, *base )
        && controllers_.GetCurrentMode() != eModes_Replay 
            ? CreateLink( state, consign.GetId() ) : state,
        state, consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::RemoveRequest
// Created: ABR 2014-09-25
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::RemoveRequest( const LogisticsConsign_ABC& consign )
{
    dataModel_.removeRow( GetRequestRow( consign ) );
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

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::NotifyUpdated
// Created: ABR 2014-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::NotifyUpdated( const kernel::Entity_ABC& element )
{
    if( !isEntityInConsignFunctor_ )
        return;
    for( int i = 0; i < model()->rowCount(); ++i )
        if( const LogisticsConsign_ABC* pRequest = GetRequest( model()->index( i, 0 ) ) )
            if( isEntityInConsignFunctor_( *pRequest, element ) )
                AddRequest( *pRequest );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetIsEntityInConsignFunctor
// Created: ABR 2014-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SetIsEntityInConsignFunctor( const T_IsEntityInConsignFunctor& functor )
{
    isEntityInConsignFunctor_ = functor;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsTable::SetRequesterNameExtractor
// Created: ABR 2014-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsTable::SetRequesterNameExtractor( const T_RequesterNameExtractor& extractor )
{
    requesterNameExtractor_ = extractor;
}
