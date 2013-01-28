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
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/ContextMenu.h"

using namespace gui;

namespace
{
    class LoggerItem : public QTreeWidgetItem
    {
    public:
        explicit LoggerItem( QTreeWidget* view) : QTreeWidgetItem( view ) {}
                ~LoggerItem() {}
        virtual bool operator < ( const QTreeWidgetItem& other ) const
        {
            return data( 0, Qt::UserRole ).toUInt() < other.data( 0, Qt::UserRole ).toUInt();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent, const kernel::Time_ABC& simulation, const std::string& filename )
    : QTreeWidget( pParent )
    , simulation_( simulation )
    , log_( filename.c_str(), std::ios::out | std::ios::app )
    , popupMenu_( new kernel::ContextMenu( this ) )
    , counter_( 0 )
{
    setBackgroundColor( Qt::white );
    setMinimumSize( 40, 40 );
    header()->setSortIndicatorShown( true );
    setRootIsDecorated( true );
    setColumnCount( 3 );
    setSortingEnabled( true );
    setHeaderHidden( false );
    setEditTriggers( 0 );
    setUniformRowHeights( true );

    QStringList headers;
    headers << tr( "Real time" ) << tr( "Simulation time" ) << tr( "Message" ) << "";
    setHeaderLabels( headers );
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
void Logger::Info( const std::string& message )
{
    MakeHeader( log_, simulation_ );
    WriteLog( message, Qt::black );
}

// -----------------------------------------------------------------------------
// Name: Logger::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void Logger::Warning( const std::string& message )
{
    MakeHeader( log_, simulation_ );
    log_ << "Warning - ";
    WriteLog( message, Qt::darkRed );
}

// -----------------------------------------------------------------------------
// Name: Logger::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void Logger::Error( const std::string& message )
{
    MakeHeader( log_, simulation_ );
    log_ << "Error - ";
    WriteLog( message, Qt::red );
}

// -----------------------------------------------------------------------------
// Name: Logger::Clear
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void Logger::Clear()
{
    counter_ = 0;
    while( topLevelItemCount() > 0 )
        delete takeTopLevelItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: Logger::WriteLog
// Created: JSR 2013-01-25
// -----------------------------------------------------------------------------
void Logger::WriteLog( const std::string& message, const QColor& color )
{
    log_ << message << std::endl;
    LoggerItem* item = new LoggerItem( this );
    item->setData( 0, Qt::UserRole, counter_++ );
    item->setText( 0, QTime::currentTime().toString() );
    item->setText( 1, simulation_.GetTimeAsString() );
    item->setText( 2, message.c_str() );
    item->setForeground( 0, color );
    item->setForeground( 1, color );
    item->setForeground( 2, color );
    insertTopLevelItem( 0, item );
}

// -----------------------------------------------------------------------------
// Name: Logger::contextMenuEvent
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void Logger::contextMenuEvent( QContextMenuEvent* event )
{
    popupMenu_->popup( event->globalPos() );
}
