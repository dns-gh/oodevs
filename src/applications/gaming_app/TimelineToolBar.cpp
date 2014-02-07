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
#include "clients_gui/FileDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/resources.h"
#include "clients_gui/SearchLineEdit.h"
#include "clients_gui/Tools.h"
#include "tools/ExerciseConfig.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( const tools::ExerciseConfig& config )
    : QToolBar( 0 )
    , config_( config )
    , filters_( tr( "Actions files (*.ord)" )  + ";;" + tr( "Timeline session files (*.timeline)" ) )
    , displayEngaged_( false )
    , displayOrders_( true )
    , displayTasks_( true )
{
    Initialize( true );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( const TimelineToolBar& other )
    : config_( other.config_ )
    , entityFilter_( other.entityFilter_ )
    , filters_( other.filters_ )
    , displayEngaged_( other.displayEngaged_ )
    , displayOrders_( other.displayOrders_ )
    , displayTasks_( other.displayTasks_ )
{
    Initialize( false );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::Initialize
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::Initialize( bool main )
{
    horizontalView_ = addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/rotate.png" ) ), tr( "Switch orientation" ), this, SIGNAL( ToggleLayoutOrientation() ) );
    filterMenu_ = new QMenu( this );
    QToolButton* button = new QToolButton( this );
    button->setIconSet( MAKE_ICON( filter ) );
    button->setTextLabel( tr( "Edit filters" ) );
    button->setPopup( filterMenu_ );
    button->setPopupMode( QToolButton::InstantPopup );
    button->setPopupDelay( 1 );
    button->setEnabled( main );
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

    filterMenu_->addAction( orderFilter );
    filterMenu_->addAction( engagedFilter_ );
    filterMenu_->addSeparator();
    filterMenu_->addAction( taskFilter );

    if( !main )
        addAction( qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), tr( "Remove current view" ), this, SIGNAL( RemoveCurrentView() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::~TimelineToolBar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnLoadOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnLoadOrderFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( "" ).Normalize();
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load actions file" ), defaultPath, filters_ );
    if( !filename.IsEmpty() && filename.Exists() && filename.IsRegularFile() )
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

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnOrderFilterToggled
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::OnOrderFilterToggled( bool toggled )
{
    displayOrders_ = toggled;
    engagedFilter_->setEnabled( displayOrders_ );
    engagedFilter_->setCheckable( displayOrders_ );
    if( !displayOrders_ )
        engagedFilter_->setChecked( false );
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
// Name: TimelineToolBar::GetServicesFilter
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
std::string TimelineToolBar::GetServicesFilter() const
{
     return "sword:" + boost::lexical_cast< std::string >( displayOrders_ )
          + ",none:" + boost::lexical_cast< std::string >( displayTasks_ );
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