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
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: TimelineToolBar constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineToolBar::TimelineToolBar( QWidget* parent, const tools::ExerciseConfig& config, bool isMain )
    : QToolBar( parent )
    , config_( config )
    , filters_( tr( "Actions files (*.ord)" )  + ";;" + tr( "Timeline session files (*.timeline)" ) )
{
    horizontalView_ = addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/rotate.png" ) ), "", this, SLOT( OnSwitchView() ) );
    horizontalView_->setCheckable( true );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/eye.png" ) ), tr( "Edit filters" ), this, SLOT( OnFilterSelection() ) );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), tr( "Center the view on the simulation time" ), this, SIGNAL( CenterView() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load actions file" ), this, SLOT( OnLoadOrderFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save actions in active timeline to file" ), this, SLOT( OnSaveOrderFile() ) );
    addSeparator();
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/new_tab.png" ) ), tr( "Create a new view" ), this, SIGNAL( AddView() ) );
    if( !isMain )
        addAction( qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), tr( "Remove current view" ), this, SIGNAL( RemoveCurrentView() ) );
    OnSwitchView();
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
    bool horizontal = horizontalView_->isChecked();
    horizontalView_->setText( horizontal ? tr( "Switch to vertical view" ) : tr( "Switch to horizontal view" ) );
    emit SetLayoutOrientation( horizontal );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnFilterSelection
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnFilterSelection()
{
    // $$$$ ABR 2013-05-28: Use a better trick than a QMenu to allow multiple selection in only one click
    QMenu menu( this );
    menu.addAction( "Add filters here" );
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
