// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineToolBar.h"
#include "moc_TimelineToolBar.cpp"
#include "clients_gui/Event.h"
#include "clients_gui/FileDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/resources.h"
#include "clients_gui/SearchLineEdit.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/ActionController.h"
#include "tools/ExerciseConfig.h"

#include <timeline/api.h>
#include <boost/algorithm/string/join.hpp>

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( kernel::Controllers& controllers,
                                  const tools::ExerciseConfig& config,
                                  const std::string& gamingUuid )
    : QToolBar( 0 )
    , eventActionsController_( controllers.eventActions_ )
    , config_( config )
    , gamingUuid_( gamingUuid )
    , filters_( tr( "All timeline files (*.ord *.timeline)" ) )
    , displayEngaged_( false )
    , displayReadOnly_( true )
    , displayOrders_( true )
    , displayTasks_( true )
    , displayMarkers_( true )
    , displaySelected_( false )
    , contextMenuEvent_( controllers )
    , filteredEntity_( controllers )
    , main_( true )
    , selected_( controllers )
{
    Initialize();
    eventActionsController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( const TimelineToolBar& other )
    : eventActionsController_( other.eventActionsController_ )
    , config_( other.config_ )
    , gamingUuid_( other.gamingUuid_ )
    , entityFilter_( other.entityFilter_ )
    , filters_( other.filters_ )
    , displayEngaged_( other.displayEngaged_ )
    , displayReadOnly_( other.displayReadOnly_ )
    , displayOrders_( other.displayOrders_ )
    , displayTasks_( other.displayTasks_ )
    , displayMarkers_( other.displayMarkers_ )
    , displaySelected_( other.displaySelected_ )
    , contextMenuEvent_( other.contextMenuEvent_ )
    , filteredEntity_( other.filteredEntity_ )
    , showOnlyFilter_( other.showOnlyFilter_ )
    , hideHierarchiesFilter_( other.hideHierarchiesFilter_ )
    , main_( false )
    , selected_( other.selected_ )
{
    Initialize();
    eventActionsController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::Initialize
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::Initialize()
{
    horizontalView_ = addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/rotate.png" ) ), tr( "Switch orientation" ), this, SIGNAL( ToggleLayoutOrientation() ) );
    filterMenu_ = new QMenu( this );
    QToolButton* button = new QToolButton( this );
    button->setIconSet( MAKE_ICON( filter ) );
    button->setTextLabel( tr( "Edit filters" ) );
    button->setPopup( filterMenu_ );
    button->setPopupMode( QToolButton::InstantPopup );
    button->setPopupDelay( 1 );
    button->setEnabled( main_ );
    addWidget( button );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), tr( "Center the view on the simulation time" ), this, SIGNAL( CenterView() ) );

    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load actions file" ), this, SLOT( OnLoadOrderFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save actions in active timeline to file" ), this, SLOT( OnSaveOrderFile() ) );

    addSeparator();
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/new_tab.png" ) ), tr( "Create a new view" ), this, SIGNAL( AddView() ) );

    addSeparator();
    auto searchWidget = new QWidget();
    auto searchLayout = new QVBoxLayout( searchWidget );
    auto searchEdit = new gui::SearchLineEdit( "timeline-keyword-edit", this );
    searchLayout->setMargin( 5 );
    searchLayout->addWidget( searchEdit );
    addWidget( searchWidget );
    connect( searchEdit, SIGNAL( textChanged( QString ) ), this, SLOT( OnFilterKeyword( QString ) ) );

    engagedFilter_ = new QAction( "    " + tr( "Display orders given to engaged units" ), this );
    connect( engagedFilter_, SIGNAL( toggled( bool ) ), this, SLOT( OnEngagedFilterToggled( bool ) ) );
    engagedFilter_->setCheckable( true );
    engagedFilter_->setChecked( displayEngaged_ );
    engagedFilter_->setEnabled( displayOrders_ );

    QAction* orderFilter = new QAction( tr( "Display orders" ), this );
    connect( orderFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnOrderFilterToggled( bool ) ) );
    orderFilter->setCheckable( true );
    orderFilter->setChecked( displayOrders_ );

    QAction* taskFilter = new QAction( tr( "Display tasks" ), this );
    connect( taskFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnTaskFilterToggled( bool ) ) );
    taskFilter->setCheckable( true );
    taskFilter->setChecked( displayTasks_ );

    QAction* markerFilter = new QAction( tr( "Display markers" ), this );
    connect( markerFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnMarkerFilterToggled( bool ) ) );
    markerFilter->setCheckable( true );
    markerFilter->setChecked( displayMarkers_ );

    QAction* selectedFilter = new QAction( tr( "Filter on selected entity" ), this );
    connect( selectedFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnSelectedFilterToggled( bool ) ) );
    selectedFilter->setCheckable( true );
    selectedFilter->setChecked( displaySelected_ );

    QAction* readOnlyFilter = new QAction( tr( "Display events on read-only entities" ), this );
    connect( readOnlyFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnReadOnlyFilterToggled( bool ) ) );
    readOnlyFilter->setCheckable( true );
    readOnlyFilter->setChecked( displayReadOnly_ );

    filterMenu_->addAction( orderFilter );
    filterMenu_->addAction( engagedFilter_ );
    filterMenu_->addAction( taskFilter );
    filterMenu_->addAction( markerFilter );
    filterMenu_->addSeparator();
    filterMenu_->addAction( selectedFilter );
    filterMenu_->addAction( readOnlyFilter );

    if( !main_ )
        addAction( qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), tr( "Remove current view" ), this, SIGNAL( RemoveCurrentView() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::~TimelineToolBar()
{
    if( main_ )
        eventActionsController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnLoadOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnLoadOrderFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( "" ).Normalize();
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load actions file" ), defaultPath, filters_ );
    if( !filename.IsEmpty() && filename.IsRegularFile() )
    {
        if( filename.Extension() == ".ord" )
            emit LoadOrderFileRequest( filename );
        if( filename.Extension() == ".timeline" )
            emit LoadTimelineSessionFileRequest( filename );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnSaveOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSaveOrderFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile(
        tools::SanitizeFileName( QString::fromStdWString( config_.GetExerciseName().BaseName().ToUnicode() ), " " ) + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) + ".ord" ).Normalize();
    tools::Path filename = gui::FileDialog::getSaveFileName( this, tr( "Save actions in active timeline to file" ), defaultPath, filters_ );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() == ".ord" )
        emit SaveOrderFileRequest( filename );
    if( filename.Extension() == ".timeline" )
        emit SaveTimelineSessionFileRequest( filename );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnEngagedFilterToggled
// Created: SLI 2013-11-20
// -----------------------------------------------------------------------------
void TimelineToolBar::OnEngagedFilterToggled( bool checked )
{
    displayEngaged_ = checked;
    emit EngagedFilterToggled( GetEngagedFilter() );
}

void TimelineToolBar::OnReadOnlyFilterToggled( bool checked )
{
    displayReadOnly_ = checked;
    emit ReadOnlyFilterToggled( checked );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnOrderFilterToggled
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::OnOrderFilterToggled( bool toggled )
{
    displayOrders_ = toggled;
    engagedFilter_->setEnabled( displayOrders_ );
    engagedFilter_->setCheckable( displayOrders_ );
    engagedFilter_->setChecked( displayOrders_ ? displayEngaged_ : false );
    emit ServicesFilterChanged( GetServicesFilter() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnTaskFilterToggled
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::OnTaskFilterToggled( bool toggled )
{
    displayTasks_ = toggled;
    emit ServicesFilterChanged( GetServicesFilter() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnMarkerFilterToggled
// Created: JSR 2014-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::OnMarkerFilterToggled( bool toggled )
{
    displayMarkers_ = toggled;
    emit ServicesFilterChanged( GetServicesFilter() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnSelectedFilterToggled
// Created: JSR 2014-11-06
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSelectedFilterToggled( bool toggled )
{
    displaySelected_ = toggled;
    emit SelectedFilterChanged();
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::SetEntityFilter
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineToolBar::SetEntityFilter( const std::string& filter )
{
    entityFilter_ = filter;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetEntityFilter
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
const std::string& TimelineToolBar::GetEntityFilter() const
{
    return entityFilter_;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetEngagedFilter
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
bool TimelineToolBar::GetEngagedFilter() const
{
    return displayEngaged_;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetSelectedFilter
// Created: JSR 2014-11-20
// -----------------------------------------------------------------------------
bool TimelineToolBar::GetSelectedFilter() const
{
    return displaySelected_;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::SetFilteredEntity
// Created: JSR 2014-11-20
// -----------------------------------------------------------------------------
void TimelineToolBar::SetFilteredEntity( const kernel::Entity_ABC* entity )
{
    filteredEntity_ = entity;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetFilteredEntity
// Created: JSR 2014-11-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* TimelineToolBar::GetFilteredEntity() const
{
    return filteredEntity_;
}

namespace
{
    std::string ConvertToFilter( bool filter, const std::string& str = "*" )
    {
        return filter ? str : "0";
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetServicesFilter
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
std::string TimelineToolBar::GetServicesFilter() const
{
    return "sword:" + ConvertToFilter( displayOrders_ )
         + ",none:" + ConvertToFilter( displayTasks_ )
         + ",marker:" + ConvertToFilter( displayMarkers_, gamingUuid_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetKeywordFilter
// Created: BAX 2013-11-25
// -----------------------------------------------------------------------------
std::string TimelineToolBar::GetKeywordFilter() const
{
    return keywordFilter_;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnFilterKeyword
// Created: BAX 2013-11-25
// -----------------------------------------------------------------------------
void TimelineToolBar::OnFilterKeyword( const QString& keyword )
{
    keywordFilter_ = keyword.toStdString();
    emit KeywordFilterChanged( keywordFilter_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetShowOnlyFilter
// Created: ABR 2014-04-16
// -----------------------------------------------------------------------------
const std::string& TimelineToolBar::GetShowOnlyFilter() const
{
    return showOnlyFilter_;
}

namespace
{
    bool IsTaskWithEnd( const gui::Event& event )
    {
        return event.GetType() == eEventTypes_Task && !event.GetEvent().end.empty();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::NotifyContextMenu
// Created: ABR 2014-04-15
// -----------------------------------------------------------------------------
void TimelineToolBar::NotifyContextMenu( const gui::Event& event, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( IsTaskWithEnd( event ) )
    {
        contextMenuEvent_ = &event;
        if( showOnlyFilter_.empty() )
        {
            menu.addSeparator();
            menu.InsertItem( "Command", tr( "Add children events" ), this, SLOT( OnAddShowOnlyFilter() ) );
        }
        menu.addSeparator();
        auto it = std::find( hideHierarchiesFilter_.begin(), hideHierarchiesFilter_.end(), event.GetEvent().uuid );
        if( it == hideHierarchiesFilter_.end() )
            menu.InsertItem( "Command", tr( "Hide children" ), this, SLOT( OnHideChildren() ) );
        else
            menu.InsertItem( "Command", tr( "Show children" ), this, SLOT( OnShowChildren() ) );
    }
    else if( showOnlyFilter_.empty() && selected_ && IsTaskWithEnd( *selected_ ) && event.GetEvent().parent != selected_->GetEvent().uuid )
    {
        contextMenuEvent_ = &event;
        menu.addSeparator();
        menu.InsertItem( "Command", tr( "Add to selected task" ), this, SLOT( OnAddToTask() ) );
    }
    if( !event.GetEvent().parent.empty() )
    {
        contextMenuEvent_ = &event;
        menu.addSeparator();
        menu.InsertItem( "Command", tr( "Remove from task" ), this, SLOT( OnRemoveFromTask() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::NotifySelected
// Created: JSR 2014-12-04
// -----------------------------------------------------------------------------
void TimelineToolBar::NotifySelected( const gui::Event* event )
{
    selected_ = event;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnAddShowOnlyFilter
// Created: ABR 2014-04-15
// -----------------------------------------------------------------------------
void TimelineToolBar::OnAddShowOnlyFilter()
{
    if( contextMenuEvent_ )
    {
        showOnlyFilter_ = contextMenuEvent_->GetEvent().uuid;
        emit ShowOnlyFilterChanged( contextMenuEvent_->GetEvent().uuid, contextMenuEvent_->GetEvent().name );
        showOnlyFilter_.clear();
    }
    contextMenuEvent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnHideChildren
// Created: ABR 2014-04-16
// -----------------------------------------------------------------------------
void TimelineToolBar::OnHideChildren()
{
    if( !contextMenuEvent_ )
        return;
    auto uuid = contextMenuEvent_->GetEvent().uuid;
    auto it = std::find( hideHierarchiesFilter_.begin(), hideHierarchiesFilter_.end(), uuid );
    if( it != hideHierarchiesFilter_.end() )
        throw MASA_EXCEPTION( "Hide hierarchy filter already enabled for uuid " + uuid );
    hideHierarchiesFilter_.insert( uuid );
    contextMenuEvent_ = 0;
    emit HideHierarchiesFilterChanged( GetHideHierarchiesFilter() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnShowChildren
// Created: ABR 2014-04-16
// -----------------------------------------------------------------------------
void TimelineToolBar::OnShowChildren()
{
    if( !contextMenuEvent_ )
        return;
    auto it = std::find( hideHierarchiesFilter_.begin(), hideHierarchiesFilter_.end(), contextMenuEvent_->GetEvent().uuid );
    if( it == hideHierarchiesFilter_.end() )
        throw MASA_EXCEPTION( "Hide hierarchy filter not found for uuid " + contextMenuEvent_->GetEvent().uuid );
    hideHierarchiesFilter_.erase( it );
    contextMenuEvent_ = 0;
    emit HideHierarchiesFilterChanged( GetHideHierarchiesFilter() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::GetHideHierarchiesFilter
// Created: ABR 2014-04-16
// -----------------------------------------------------------------------------
std::string TimelineToolBar::GetHideHierarchiesFilter() const
{
    return boost::algorithm::join( hideHierarchiesFilter_, "," );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnAddToTask
// Created: JSR 2014-12-04
// -----------------------------------------------------------------------------
void TimelineToolBar::OnAddToTask()
{
    if( !contextMenuEvent_ || !selected_ )
        return;
    emit ParentChanged( contextMenuEvent_->GetEvent().uuid, selected_->GetEvent().uuid );
    contextMenuEvent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnRemoveFromTask
// Created: JSR 2014-12-04
// -----------------------------------------------------------------------------
void TimelineToolBar::OnRemoveFromTask()
{
    if( !contextMenuEvent_ )
        return;
    emit ParentChanged( contextMenuEvent_->GetEvent().uuid, "" );
    contextMenuEvent_ = 0;
}
