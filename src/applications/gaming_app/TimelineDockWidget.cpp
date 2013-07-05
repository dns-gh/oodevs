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
#include "EventDialog.h"
#include "TimelineToolBar.h"
#include "TimelineWebView.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/Model.h"
#include "MT_Tools/MT_Logger.h"
#include "timeline/api.h"

int TimelineDockWidget::maxTabNumber_ = -1;

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget constructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const kernel::Time_ABC& simulation,
                                        Model& model, actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile,
                                        gui::GlTools_ABC& tools )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , cfg_( new timeline::Configuration() )
    , config_( config )
    , eventDialog_( 0 )
    , webView_( 0 )
{
    // Init
    setCaption( tr( "Actions timeline" ) );

    // Configuration
    cfg_->url = "http://" + config.GetTimelineUrl() + "/?lang=" + tools::readLang(); // $$$$ ABR 2013-05-24: Timeline server must keep this parameter when it automatically add session id
    cfg_->debug_port = config.GetTimelineDebugPort();
    cfg_->rundir = "cef";
    cfg_->binary = "cef/timeline_client.exe";
    cfg_->external = true;
    if( !cfg_->binary.IsRegularFile() )
        MT_LOG_ERROR_MSG( tr( "Invalid timeline binary '%1'" ).arg( QString::fromStdWString( cfg_->binary.ToUnicode() ) ).toStdString() );

    // Dialog
    eventDialog_ = new EventDialog( this, controllers, model, config, simulation, interfaceBuilder, profile, tools );

    // Tabbed toolbar
    tabWidget_ = new QTabWidget();
    tabWidget_->setVisible( false );

    webView_ = new TimelineWebView( 0, config, controllers.actions_, simulation, model, *eventDialog_, *cfg_ );

    // Add first toolbar
    AddView();

    // Main Layout
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainLayout->addWidget( tabWidget_ );
    mainLayout->addWidget( webView_, 1 );
    setWidget( mainWidget );

    // Connections
    connect( eventDialog_, SIGNAL( CreateEvent( const timeline::Event& ) ), webView_, SLOT( CreateEvent( const timeline::Event& ) ) );
    connect( eventDialog_, SIGNAL( EditEvent( const timeline::Event& ) ), webView_, SLOT( EditEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( CreateEvent( const timeline::Event& ) ), webView_, SLOT( CreateEvent( const timeline::Event& ) ) );
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
    tabWidget_->setVisible( true );
    if( webView_ )
        webView_->Connect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Disconnect
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::Disconnect()
{
    tabWidget_->setVisible( false );
    if( webView_ )
        webView_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void TimelineDockWidget::Draw( gui::Viewport_ABC& viewport )
{
    if( eventDialog_ )
        eventDialog_->Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::AddFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::AddView()
{
    TimelineToolBar* toolBar = new TimelineToolBar( 0, config_, ++maxTabNumber_ == 0 );
    connect( toolBar, SIGNAL( AddView() ), this, SLOT( AddView() ) );
    connect( toolBar, SIGNAL( RemoveCurrentView() ), this, SLOT( RemoveCurrentView() ) );
    connect( toolBar, SIGNAL( LoadOrderFileRequest( const tools::Path& ) ), webView_, SLOT( OnLoadOrderFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( SaveOrderFileRequest( const tools::Path& ) ), webView_, SLOT( OnSaveOrderFileRequested( const tools::Path& ) ) );

    tabWidget_->addTab( toolBar, ( maxTabNumber_ == 0 ) ? tr( "Main" ) : tr( "View %1" ).arg( maxTabNumber_ ) );
    toolbars_.push_back( std::make_pair( maxTabNumber_, toolBar ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::RemoveCurrentFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::RemoveCurrentView()
{
    int currentIndex = tabWidget_->currentIndex();
    if( currentIndex != 0 )
    {
        QWidget* currentWidget = tabWidget_->currentWidget();
        maxTabNumber_ = -1;
        for( auto it = toolbars_.begin(); it != toolbars_.end(); )
            if( it->second == currentWidget )
                it = toolbars_.erase( it );
            else
            {
                if( maxTabNumber_ < it->first )
                    maxTabNumber_ = it->first;
                ++it;
            }
        tabWidget_->removeTab( currentIndex );
    }
}
