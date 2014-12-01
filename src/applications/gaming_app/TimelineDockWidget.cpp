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
#include "GamingConfig.h"
#include "TimelineToolBar.h"
#include "TimelineWebView.h"
#include "clients_gui/Event.h"
#include "clients_gui/VisibilityFilter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Model.h"
#include "gaming/Profile.h"
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
                                        const GamingConfig& config,
                                        Model& model,
                                        gui::GLWidgetManager& glWidgetManager,
                                        const kernel::Profile_ABC& profile )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , config_( config )
    , gamingUuid_( model.GetUuid() )
    , mainView_( 0 )
    , selectedEntity_( controllers )
{
    // Init
    setCaption( tr( "Timeline" ) );

    // Content
    tabWidget_ = new QTabWidget();
    tabWidget_->setVisible( false );
    tabWidget_->setContextMenuPolicy( Qt::CustomContextMenu );
    tabWidget_->setMovable( true );
    contextMenu_ = new QMenu( this );
    contextMenu_->addAction( tr( "Rename view" ), this, SLOT( OnRenameTab() ) );
    webView_.reset( new TimelineWebView( 0, config, controllers, model, glWidgetManager, profile ) );

    // Main Layout
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainLayout->addWidget( tabWidget_ );
    mainLayout->addWidget( webView_.get(), 1 );
    setWidget( mainWidget );

    // Connections
    connect( tabWidget_, SIGNAL( currentChanged( int ) ), this, SLOT( OnCurrentChanged( int ) ) );
    connect( tabWidget_, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnTabContextMenu() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget destructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::~TimelineDockWidget()
{
    webView_->reparent( 0, QPoint( 0, 0 ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::GetWebView
// Created: ABR 2013-12-06
// -----------------------------------------------------------------------------
const boost::shared_ptr< TimelineWebView >& TimelineDockWidget::GetWebView() const
{
    return webView_;
}

void TimelineDockWidget::NotifyUpdated( const Profile& profile )
{
    if( !profile.IsLoggedIn() || mainView_ )
        return;
    mainView_ = AddView( true );
    connect( mainView_, SIGNAL( ShowOnlyFilterChanged( const std::string&, const std::string& ) ),
                        SLOT( OnShowOnlyFilterChanged( const std::string&, const std::string& ) ) );
    tabWidget_->setVisible( true );
    webView_->Connect();
}

void TimelineDockWidget::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    tabWidget_->setVisible( false );
    tabWidget_->clear();
    mainView_ = 0;
    maxTabNumber_= 0;
    webView_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::AddFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar* TimelineDockWidget::AddView( bool main /* = false */,
                                      const std::string& name /* = "" */ )
{
    TimelineToolBar* toolBar = main ? new TimelineToolBar( controllers_, config_, gamingUuid_ ) : new TimelineToolBar( *static_cast< TimelineToolBar* >( tabWidget_->widget( 0 ) ) );
    connect( toolBar, SIGNAL( CenterView() ), webView_.get(), SLOT( OnCenterView() ) );
    connect( toolBar, SIGNAL( AddView() ), this, SLOT( AddView() ) );
    connect( toolBar, SIGNAL( RemoveCurrentView() ), this, SLOT( RemoveCurrentView() ) );
    connect( toolBar, SIGNAL( LoadOrderFileRequest( const tools::Path& ) ), webView_.get(), SLOT( OnLoadOrderFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( LoadOrderFileRequest( const tools::Path& ) ), this, SLOT( OnLoadRequested() ) );
    connect( toolBar, SIGNAL( SaveOrderFileRequest( const tools::Path& ) ), webView_.get(), SLOT( OnSaveOrderFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( LoadTimelineSessionFileRequest( const tools::Path& ) ), webView_.get(), SLOT( OnLoadTimelineSessionFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( LoadTimelineSessionFileRequest( const tools::Path& ) ), this, SLOT( OnLoadRequested() ) );
    connect( toolBar, SIGNAL( SaveTimelineSessionFileRequest( const tools::Path& ) ), webView_.get(), SLOT( OnSaveTimelineSessionFileRequested( const tools::Path& ) ) );
    connect( toolBar, SIGNAL( ToggleLayoutOrientation() ), webView_.get(), SLOT( OnToggleLayoutOrientation() ) );
    connect( toolBar, SIGNAL( EngagedFilterToggled( bool ) ), webView_.get(), SLOT( OnEngagedFilterToggled( bool ) ) );
    connect( toolBar, SIGNAL( ReadOnlyFilterToggled( bool ) ), webView_.get(), SLOT( OnReadOnlyFilterToggled( bool ) ) );
    connect( toolBar, SIGNAL( ServicesFilterChanged( const std::string& ) ), webView_.get(), SLOT( OnServicesFilterChanged( const std::string& ) ) );
    connect( toolBar, SIGNAL( KeywordFilterChanged( const std::string& ) ), webView_.get(), SLOT( OnKeywordFilterChanged( const std::string& ) ) );
    connect( toolBar, SIGNAL( HideHierarchiesFilterChanged( const std::string& ) ), webView_.get(), SLOT( OnHideHierarchiesFilterChanged( const std::string& ) ) );
    connect( toolBar, SIGNAL( SelectedFilterChanged() ), this, SLOT( OnSelectedFilterChanged() ) );
    const int index = tabWidget_->addTab( toolBar, "" );
    tabWidget_->setTabText( index, main ? tr( "Main" ) : name.empty() ? tr( "View %1" ).arg( ++maxTabNumber_ ) : QString::fromStdString( name ) );
    tabWidget_->setCurrentIndex( index );
    return toolBar;
}

namespace
{
    std::map< std::string, QWidget* >::const_iterator FindShowOnlyWidget( const std::map< std::string, QWidget* >& showOnlyViews,
                                                                          QWidget* widget )
    {
        return std::find_if( showOnlyViews.begin(),
                             showOnlyViews.end(),
                             [&]( const std::pair< const std::string, QWidget* >& element ) {
                                 return element.second == widget;
                             } );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::RemoveCurrentFilteredView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::RemoveCurrentView()
{
    int currentIndex = tabWidget_->currentIndex();
    if( currentIndex != tabWidget_->indexOf( mainView_ ) )
    {
        auto widget = tabWidget_->widget( currentIndex );
        auto it = FindShowOnlyWidget( showOnlyViews_, widget );
        if( it != showOnlyViews_.end() )
            showOnlyViews_.erase( it );
        tabWidget_->removeTab( currentIndex );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCurrentChanged
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnCurrentChanged( int index )
{
    if( auto toolbar = dynamic_cast< TimelineToolBar* >( tabWidget_->widget( index ) ) )
        UpdateWebView( *toolbar );
}

namespace
{
    std::string GetEntityFilter( const kernel::Entity_ABC* entity )
    {
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
// Name: TimelineDockWidget::GetEntityFilter
// Created: JSR 2014-11-07
// -----------------------------------------------------------------------------
std::string TimelineDockWidget::GetEntityFilter( const TimelineToolBar& toolbar ) const
{
    const kernel::Entity_ABC* filteredEntity = toolbar.GetFilteredEntity();
    if( !toolbar.GetSelectedFilter() )
        return ::GetEntityFilter( filteredEntity );
    std::string result;
    if( selectedEntity_ )
    {
        if( !filteredEntity || selectedEntity_ == filteredEntity )
            result = ::GetEntityFilter( selectedEntity_ );
        else
        {
            const kernel::TacticalHierarchies* hFiltered = filteredEntity->Retrieve< kernel::TacticalHierarchies >();
            const kernel::TacticalHierarchies* hSelected = selectedEntity_->Retrieve< kernel::TacticalHierarchies >();
            if( hFiltered && hFiltered->IsSubordinateOf( *selectedEntity_ ) )
                result = ::GetEntityFilter( filteredEntity );
            else if( hSelected && hSelected->IsSubordinateOf( *filteredEntity ) )
                result = ::GetEntityFilter( selectedEntity_ );
        }
    }
    return result.empty() ? "u:0" : result;
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCurrentChanged
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget::NotifyCreated( const gui::VisibilityFilter_ABC& filter )
{
    NotifyUpdated( filter );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::NotifyUpdated
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineDockWidget:: NotifyUpdated( const gui::VisibilityFilter_ABC& filter )
{
    if( mainView_ )
    {
        mainView_->SetFilteredEntity( filter.GetFilteredEntity() );
        mainView_->SetEntityFilter( GetEntityFilter( *mainView_ ) );
        UpdateWebView( *mainView_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnTabContextMenu
// Created: SLI 2013-11-22
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnTabContextMenu()
{
    if( tabWidget_->currentIndex() == tabWidget_->indexOf( mainView_ ) )
        return;
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
    {
        auto it = FindShowOnlyWidget( showOnlyViews_, tabWidget_->widget( index ) );
        if( it == showOnlyViews_.end() )
            // normal tab, just rename it
            tabWidget_->setTabText( index, text );
        else
            // show only tab, rename the event (the tab will be renamed by NotifyUpdated( const gui::Event& ))
            webView_->RenameEvent( it->first, text.toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnLoadRequested
// Created: SLI 2013-11-28
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnLoadRequested()
{
    tabWidget_->setCurrentIndex( tabWidget_->indexOf( mainView_ ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnShowOnlyFilterChanged
// Created: ABR 2014-04-16
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnShowOnlyFilterChanged( const std::string& uuid, const std::string& name )
{
    auto it = showOnlyViews_.find( uuid );
    if( it == showOnlyViews_.end() )
        showOnlyViews_[ uuid ] = AddView( false, name );
    else
        tabWidget_->setCurrentIndex( tabWidget_->indexOf( it->second ) );
    webView_->OnShowOnlyFilterChanged( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnSelectedFilterChanged
// Created: JSR 2014-11-06
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnSelectedFilterChanged()
{
    if( mainView_ )
    {
        mainView_->SetEntityFilter( GetEntityFilter( *mainView_ ) );
        UpdateWebView( *mainView_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::NotifyUpdated
// Created: ABR 2014-05-13
// -----------------------------------------------------------------------------
void TimelineDockWidget::NotifyUpdated( const gui::Event& event )
{
    auto it = showOnlyViews_.find( event.GetEvent().uuid );
    if( it != showOnlyViews_.end() )
    {
        auto index = tabWidget_->indexOf( it->second );
        tabWidget_->setTabText( index, QString::fromStdString( event.GetEvent().name ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::BeforeSelection
// Created: JSR 2014-11-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::BeforeSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Select
// Created: JSR 2014-11-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::Select( const kernel::Entity_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Select
// Created: JSR 2014-11-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::Select( const actions::Action_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::AfterSelection
// Created: JSR 2014-11-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::AfterSelection()
{
    for( int i = 0; i < tabWidget_->count(); ++i )
    {
        TimelineToolBar* toolbar = dynamic_cast< TimelineToolBar* >( tabWidget_->widget( i ) );
        if( toolbar && toolbar->GetSelectedFilter() )
        {
            toolbar->SetEntityFilter( GetEntityFilter( *toolbar ) );
            UpdateWebView( *toolbar );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::UpdateWebView
// Created: JSR 2014-11-06
// -----------------------------------------------------------------------------
void TimelineDockWidget::UpdateWebView( TimelineToolBar& toolbar ) const
{
    if( tabWidget_->currentIndex() == tabWidget_->indexOf( &toolbar ) )
        webView_->UpdateFilters( toolbar.GetEntityFilter(),
                                 toolbar.GetEngagedFilter(),
                                 toolbar.GetServicesFilter(),
                                 toolbar.GetKeywordFilter(),
                                 toolbar.GetHideHierarchiesFilter(),
                                 toolbar.GetShowOnlyFilter() );
}
