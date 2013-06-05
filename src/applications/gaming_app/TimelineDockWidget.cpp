// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineDockWidget.h"
#include "moc_TimelineDockWidget.cpp"
#include "Config.h"
#include "TimelineFilteredViewWidget.h"

#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_Logger.h"
#include "timeline/api.h"

int TimelineDockWidget::maxTabNumber_ = -1;

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget constructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const Simulation& simulation )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , cfg_( new timeline::Configuration() )
    , simulation_( simulation )
    , isConnected_( false )
{
    setCaption( tr( "Actions timeline" ) );
    mainTitle_ = tr( "Main" );

    // Configuration
    cfg_->url = "http://" + config.GetTimelineUrl() + "/?lang=" + tools::readLang(); // $$$$ ABR 2013-05-24: Timeline server must keep this parameter when it automatically add session id
    cfg_->debug_port = config.GetTimelineDebugPort();
    cfg_->rundir = "cef";
    cfg_->binary = "cef/timeline_client.exe";
    cfg_->external = true;

    if( !cfg_->binary.IsRegularFile() )
        MT_LOG_ERROR_MSG( tr( "Invalid timeline binary '%1'" ).arg( QString::fromStdWString( cfg_->binary.ToUnicode() ) ).toStdString() );

    tabWidget_ = new QTabWidget( this );
    setWidget( tabWidget_ );

    QStringList filters;
    filters << "all";
    AddFilteredView( filters ); // Main tab
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget destructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::~TimelineDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Connect
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::Connect()
{
    isConnected_ = true;
    for( auto it = filteredViews_.begin(); it != filteredViews_.end(); ++it )
        if( *it )
            ( *it )->Connect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Disconnect
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::Disconnect()
{
    isConnected_ = false;
    for( auto it = filteredViews_.begin(); it != filteredViews_.end(); ++it )
        if( *it )
            ( *it )->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::AddFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::AddFilteredView( QStringList filters )
{
    TimelineFilteredViewWidget* filteredView = new TimelineFilteredViewWidget( tabWidget_, simulation_, *cfg_, ++maxTabNumber_, filters );
    tabWidget_->addTab( filteredView, ( maxTabNumber_ == 0 ) ? mainTitle_ : tr( "View %1" ).arg( maxTabNumber_ ) );
    connect( filteredView, SIGNAL( AddNewFilteredView( const QStringList& ) ), this, SLOT( AddFilteredView( const QStringList& ) ) );
    connect( filteredView, SIGNAL( RemoveCurrentFilteredView() ), this, SLOT( RemoveCurrentFilteredView() ) );
    filteredViews_.push_back( filteredView );
    if( isConnected_ )
        filteredView->Connect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::RemoveCurrentFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::RemoveCurrentFilteredView()
{
    int currentIndex = tabWidget_->currentIndex();
    if( tabWidget_->tabText( currentIndex ) != mainTitle_ )
    {
        QWidget* currentWidget = tabWidget_->currentWidget();
        auto it = std::find( filteredViews_.begin(), filteredViews_.end(), currentWidget );
        if( it != filteredViews_.end() )
        {
            ( *it )->Disconnect();
            filteredViews_.erase( it );
        }
        tabWidget_->removeTab( currentIndex );
    }
    maxTabNumber_ = -1;
    for( auto it = filteredViews_.begin(); it != filteredViews_.end(); ++it )
        if( maxTabNumber_ < ( *it )->GetViewNumber() )
            maxTabNumber_ = ( *it )->GetViewNumber();
}
