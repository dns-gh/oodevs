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
#include "TimelineToolBar.h"
#include "TimelineWebView.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Model.h"
#include "MT_Tools/MT_Logger.h"
#include <timeline/api.h>
#include <boost/lexical_cast.hpp>

int TimelineDockWidget::maxTabNumber_ = 0;

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget constructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::TimelineDockWidget( QWidget* parent,
                                        kernel::Controllers& controllers,
                                        const Config& config,
                                        Model& model )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , cfg_( new timeline::Configuration() )
    , config_( config )
    , webView_( 0 )
    , mainView_( 0 )
{
    // Init
    setCaption( tr( "Timeline" ) );

    // Configuration
    cfg_->url = "http://" + config.GetTimelineUrl();
    int timelineDebugPort = config.GetTimelineDebugPort();
    if( timelineDebugPort != 0 )
        cfg_->debug_port = timelineDebugPort;
    cfg_->rundir = "cef";
    cfg_->binary = "cef/timeline_client.exe";
    cfg_->external = true;
    if( !cfg_->binary.IsRegularFile() )
        MT_LOG_ERROR_MSG( tr( "Invalid timeline binary '%1'" ).arg( QString::fromStdWString( cfg_->binary.ToUnicode() ) ).toStdString() );

    // Content
    tabWidget_ = new QTabWidget();
    tabWidget_->setVisible( false );
    tabWidget_->setContextMenuPolicy( Qt::CustomContextMenu );
    tabWidget_->setMovable( true );
    contextMenu_ = new QMenu( this );
    contextMenu_->addAction( tr( "Rename view" ), this, SLOT( OnRenameTab() ) );

    webView_ = new TimelineWebView( 0, config, controllers, model, *cfg_ );

    // Main Layout
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainLayout->addWidget( tabWidget_ );
    mainLayout->addWidget( webView_, 1 );
    setWidget( mainWidget );

    // Connections
    connect( this, SIGNAL( CreateEvent( const timeline::Event& ) ), webView_, SLOT( CreateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( SelectEvent( const std::string& ) ), webView_, SLOT( SelectEvent( const std::string& ) ) );
    connect( this, SIGNAL( EditEvent( const timeline::Event& ) ), webView_, SLOT( EditEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( DeleteEvent( const std::string& ) ), webView_, SLOT( DeleteEvent( const std::string& ) ) );
    connect( tabWidget_, SIGNAL( currentChanged( int ) ), this, SLOT( OnCurrentChanged( int ) ) );
    connect( tabWidget_, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnTabContextMenu() ) );
    connect( webView_, SIGNAL( StartCreation( E_EventTypes, const QDateTime&, bool ) ), this, SIGNAL( StartCreation( E_EventTypes, const QDateTime&, bool ) ) );
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
    mainView_ = AddView( true );
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
    tabWidget_->clear();
    mainView_ = 0;
    maxTabNumber_= 0;
    if( webView_ )
        webView_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::AddFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
QWidget* TimelineDockWidget::AddView( bool main )
{
    TimelineToolBar* toolBar = main ? new TimelineToolBar( config_ ) : new TimelineToolBar( *static_cast< TimelineToolBar* >( tabWidget_->widget( 0 ) ) );
    connect( toolBar, SIGNAL( CenterView() ), webView_, SLOT( OnCenterView() ) );
    connect( toolBar, SIGNAL( AddView() ), this, SLOT( AddView() ) );
    connect( toolBar, SIGNAL( RemoveCurrentView() ), this, SLOT( RemoveCurrentView() ) );
    connect( toolBar, SIGNAL( LoadOrderFileRequest( const tools::Path& ) ), webView_, SLOT( OnLoadOrderFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( SaveOrderFileRequest( const tools::Path& ) ), webView_, SLOT( OnSaveOrderFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( LoadTimelineSessionFileRequest( const tools::Path& ) ), webView_, SLOT( OnLoadTimelineSessionFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( SaveTimelineSessionFileRequest( const tools::Path& ) ), webView_, SLOT( OnSaveTimelineSessionFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( SetLayoutOrientation( bool ) ), webView_, SLOT( OnSetLayoutOrientation( bool ) ) );
    connect( toolBar, SIGNAL( EngagedFilterToggled( bool ) ), webView_, SLOT( OnEngagedFilterToggled( bool ) ) );
    connect( toolBar, SIGNAL( ServicesFilterChanged( const std::string& ) ), webView_, SLOT( OnServicesFilterChanged( const std::string& ) ) );
    connect( toolBar, SIGNAL( KeywordFilterChanged( const std::string& ) ), webView_, SLOT( OnKeywordFilterChanged( const std::string& ) ) );
    const int index = tabWidget_->addTab( toolBar, "" );
    tabWidget_->setTabText( index, main ? tr( "Main" ): tr( "View %1" ).arg( ++maxTabNumber_ ) );
    tabWidget_->setCurrentIndex( index );
    return tabWidget_->widget( index );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::RemoveCurrentFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::RemoveCurrentView()
{
    int currentIndex = tabWidget_->currentIndex();
    if( currentIndex != tabWidget_->indexOf( mainView_ ) )
        tabWidget_->removeTab( currentIndex );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCurrentChanged
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnCurrentChanged( int index )
{
    if( TimelineToolBar* toolbar = static_cast< TimelineToolBar* >( tabWidget_->widget( index ) ) )
        webView_->UpdateFilters( toolbar->GetEntityFilter(),
                                 toolbar->GetEngagedFilter(),
                                 toolbar->GetServicesFilter(),
                                 toolbar->GetKeywordFilter() );
}

namespace
{
    std::string GetEntityFilter( const kernel::Filter_ABC& filter )
    {
        auto entity = filter.GetFilteredEntity();
        if( !entity )
            return std::string();
        const auto& type = entity->GetTypeName();
        std::string query;
        if( type == kernel::Agent_ABC::typeName_ )
            query = "u:";
        else if( type == kernel::Automat_ABC::typeName_ )
            query = "a:";
        else if( type == kernel::Formation_ABC::typeName_ )
            query = "f:";
        else if( type == kernel::Team_ABC::typeName_ )
            query = "p:";
        else if( type == kernel::Inhabitant_ABC::typeName_ )
            query = "i:";
        else if( type == kernel::Population_ABC::typeName_ )
            query = "c:";
        else
            MT_LOG_ERROR_MSG( "unsupported entity type " + type );
        if( !query.empty() )
            query += boost::lexical_cast< std::string >( entity->GetId() );
        return query;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCurrentChanged
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget::NotifyCreated( const kernel::Filter_ABC& filter )
{
    NotifyUpdated( filter );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCurrentChanged
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget:: NotifyUpdated( const kernel::Filter_ABC& filter )
{
    if( TimelineToolBar* main = static_cast< TimelineToolBar* >( mainView_ ) )
    {
        main->SetEntityFilter( GetEntityFilter( filter ) );
        if( tabWidget_->currentIndex() == tabWidget_->indexOf( mainView_ ) )
            webView_->UpdateFilters( main->GetEntityFilter(),
                                     main->GetEngagedFilter(),
                                     main->GetServicesFilter(),
                                     main->GetKeywordFilter() );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnTabContextMenu
// Created: SLI 2013-11-22
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnTabContextMenu()
{
    contextMenu_->exec( QCursor::pos() );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnRenameTab
// Created: SLI 2013-11-22
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnRenameTab()
{
    const int index = tabWidget_->currentIndex();
    bool ok;
    const QString text = QInputDialog::getText( this, tr( "Rename view" ), tr( "Name" ), QLineEdit::Normal,
                                                tabWidget_->tabText( index ), &ok );
    if( ok && !text.stripWhiteSpace().isEmpty() )
        tabWidget_->setTabText( index, text );
}
