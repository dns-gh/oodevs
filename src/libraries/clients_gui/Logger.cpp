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
#include "SubObjectName.h"
#include "RichWidget.h"
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
Logger::Logger( QWidget* pParent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, const tools::Path& filename )
    : gui::RichDockWidget( controllers, pParent, "logger-panel" )
    , simulation_( simulation )
    , log_( filename, std::ios::out | std::ios::app )
    , popupMenu_( new kernel::ContextMenu( this ) )
    , counter_( 0 )
{
    SubObjectName subObject( this->objectName() );
    setWindowTitle( tr( "Log" ) );
    treeWidget_ = new gui::RichWidget< QTreeWidget >( "treeWidget", this );
    setWidget( treeWidget_ );

    treeWidget_->setBackgroundColor( Qt::white );
    treeWidget_->setMinimumSize( 40, 40 );
    treeWidget_->header()->setSortIndicatorShown( true );
    treeWidget_->setRootIsDecorated( true );
    treeWidget_->setColumnCount( 3 );
    treeWidget_->setSortingEnabled( true );
    treeWidget_->setHeaderHidden( false );
    treeWidget_->setEditTriggers( 0 );
    treeWidget_->setUniformRowHeights( true );

    QStringList headers;
    headers << tr( "Real time" ) << tr( "Simulation time" ) << tr( "Message" ) << "";
    treeWidget_->setHeaderLabels( headers );
    treeWidget_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    treeWidget_->header()->setResizeMode( 1, QHeaderView::ResizeToContents );
    treeWidget_->header()->setResizeMode( 2, QHeaderView::ResizeToContents );

    treeWidget_->setAllColumnsShowFocus( true );

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
    while( treeWidget_->topLevelItemCount() > 0 )
        delete treeWidget_->takeTopLevelItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: Logger::WriteLog
// Created: JSR 2013-01-25
// -----------------------------------------------------------------------------
void Logger::WriteLog( const std::string& message, const QColor& color )
{
    log_ << message << std::endl;
    LoggerItem* item = new LoggerItem( treeWidget_ );
    item->setData( 0, Qt::UserRole, counter_++ );
    item->setText( 0, QTime::currentTime().toString() );
    item->setText( 1, simulation_.GetTimeAsString() );
    item->setText( 2, message.c_str() );
    item->setForeground( 0, color );
    item->setForeground( 1, color );
    item->setForeground( 2, color );
    treeWidget_->insertTopLevelItem( 0, item );
}

// -----------------------------------------------------------------------------
// Name: Logger::contextMenuEvent
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void Logger::contextMenuEvent( QContextMenuEvent* event )
{
    popupMenu_->popup( event->globalPos() );
}
