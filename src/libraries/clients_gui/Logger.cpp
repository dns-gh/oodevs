// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"
#include "clients_kernel\Time_ABC.h"
#include "clients_kernel\ContextMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent, const kernel::Time_ABC& simulation, const std::string& filename )
    : RichTreeView( pParent )
    , simulation_( simulation )
    , log_( filename.c_str(), std::ios::out | std::ios::app )
    , popupMenu_( new kernel::ContextMenu( this ) )
    , counter_( 0 )
{
    setBackgroundColor( Qt::white );
    setMinimumSize( 40, 40 );
    header()->setSortIndicatorShown( true );
    setRootIsDecorated( true );
    dataModel_.setColumnCount( 4 );
    setHeaderHidden( false );
    setEditTriggers( 0 );
    EnableDragAndDrop( false );

    QStringList headers;
    headers << tr( "Real time" ) << tr( "Simulation time" ) << tr( "Message" ) << "";
    dataModel_.setHorizontalHeaderLabels( headers );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    header()->setResizeMode( 1, QHeaderView::ResizeToContents );
    header()->setResizeMode( 2, QHeaderView::ResizeToContents );

    setAllColumnsShowFocus( true );

    popupMenu_->insertItem( tr( "Clear list" ), this, SLOT( Clear() ) );
}

namespace
{
    void MakeHeader( std::ostream& s, const kernel::Time_ABC& simulation )
    {
        s << "[" << QTime::currentTime().toString() << "] [" << simulation.GetTimeAsString() << "] ";
    }
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    MakeHeader( log_, simulation_ );
    log_ << "----------------------------------------------------------------" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::Info
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Info()
{
    MakeHeader( log_, simulation_ );
    return StartLog( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: Logger::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Warning()
{
    MakeHeader( log_, simulation_ );
    log_ << "Warning - ";
    return StartLog( Qt::darkRed );
}

// -----------------------------------------------------------------------------
// Name: Logger::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Error()
{
    MakeHeader( log_, simulation_ );
    log_ << "Error - ";
    return StartLog( Qt::red );
}

// -----------------------------------------------------------------------------
// Name: Logger::Clear
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void Logger::Clear()
{
    counter_ = 0;
    dataModel_.removeRows( 0, dataModel_.rowCount() );
}

// -----------------------------------------------------------------------------
// Name: Logger::StartLog
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::StartLog( const QColor& color )
{
    std::stringstream* output = new std::stringstream();
    const int rowCount = dataModel_.rowCount();
    QStandardItem* item = dataModel_.AddRootDataItem( rowCount, 0, QTime::currentTime().toString(), "", *output );
    item->setForeground( color );
    item->setData( counter_++, Roles::OtherRole );
    dataModel_.AddRootTextItem( rowCount, 1, simulation_.GetTimeAsString(), "" )->setForeground( color );
    dataModel_.AddRootItem( rowCount, 2 )->setForeground( color );
    return LogElement( *this, *output );
}

// -----------------------------------------------------------------------------
// Name: Logger::End
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void Logger::End( std::stringstream& output )
{
    log_ << output.str() << std::endl;
    QStandardItem* item = dataModel_.FindDataItem( output, dataModel_.invisibleRootItem() );
    if( item )
    {
        item->setData( *new QVariant(), Roles::DataRole );
        QModelIndex index = dataModel_.index( item->row(), 2, dataModel_.indexFromItem( item->parent() ) );
        QStandardItem* msgItem = dataModel_.GetItemFromIndex( index );
        if( msgItem )
            msgItem->setText( output.str().c_str() );
    }
    delete &output;
}

// -----------------------------------------------------------------------------
// Name: Logger::contextMenuEvent
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void Logger::contextMenuEvent( QContextMenuEvent* event )
{
    popupMenu_->popup( event->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: Logger::LessThan
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
bool Logger::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    QStandardItem* itemLeft = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( left ) );
    QStandardItem* itemRight = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( right ) );
    if( itemLeft && itemRight)
    {
        valid = true;
        return itemLeft->data( Roles::OtherRole ).toUInt() < itemRight->data( Roles::OtherRole ).toUInt();
    }
    return false;
}
