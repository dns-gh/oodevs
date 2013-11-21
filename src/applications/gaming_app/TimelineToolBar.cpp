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
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( const tools::ExerciseConfig& config )
    : QToolBar( 0 )
    , config_( config )
    , filters_( tr( "Actions files (*.ord)" )  + ";;" + tr( "Timeline session files (*.timeline)" ) )
    , displayEngaged_( false )
    , horizontalMode_( true )
{
    Initialize();
    OnSwitchView();
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
    , horizontalMode_( other.horizontalMode_ )
{
    Initialize();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), tr( "Remove current view" ), this, SIGNAL( RemoveCurrentView() ) );
    OnSwitchView();
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::Initialize
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineToolBar::Initialize()
{
    horizontalView_ = addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/rotate.png" ) ), "", this, SLOT( OnSwitchView() ) );
    addAction( MAKE_ICON( filter ), tr( "Edit filters" ), this, SLOT( OnFilterSelection() ) );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), tr( "Center the view on the simulation time" ), this, SIGNAL( CenterView() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load actions file" ), this, SLOT( OnLoadOrderFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save actions in active timeline to file" ), this, SLOT( OnSaveOrderFile() ) );
    addSeparator();
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/new_tab.png" ) ), tr( "Create a new view" ), this, SIGNAL( AddView() ) );
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
// Name: TimelineToolBar::OnSwitchView
// Created: ABR 2013-10-25
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSwitchView()
{
    horizontalMode_ = !horizontalMode_;
    horizontalView_->setText( horizontalMode_ ? tr( "Switch to vertical view" ) : tr( "Switch to horizontal view" ) );
    emit SetLayoutOrientation( horizontalMode_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnFilterSelection
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnFilterSelection()
{
    QMenu menu( this );
    QAction* engagedFilter = new QAction( tr( "Display engaged units" ), this );
    connect( engagedFilter, SIGNAL( toggled( bool ) ), this, SLOT( OnEngagedFilterToggled( bool ) ) );
    engagedFilter->setCheckable( true );
    engagedFilter->setChecked( displayEngaged_ );
    menu.addAction( engagedFilter );
    menu.exec( QCursor::pos() );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnLoadOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnLoadOrderFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) + ".ord" );
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
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) );
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
