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
{
    addAction( qApp->style()->standardIcon( QStyle::SP_BrowserReload ), tr( "Center the view on the simulation time" ), this, SLOT( OnCenterView() ) );
    addAction( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/eye.png" ) ), tr( "Edit filters" ), this, SLOT( OnFilterSelection() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load actions file" ), this, SLOT( OnLoadOrderFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save actions in active timeline to file" ), this, SLOT( OnSaveOrderFile() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOpenButton ), tr( "Load timeline session file" ), this, SLOT( OnLoadTimelineSessionFile() ) );
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save timeline session to file" ), this, SLOT( OnSaveTimelineSessionFile() ) );
    addSeparator();
    addAction( qApp->style()->standardIcon( QStyle::SP_DialogOkButton ), tr( "Create a new view" ), this, SIGNAL( AddView() ) );
    if( !isMain )
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
// Name: TimelineToolBar::OnCenterView
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineToolBar::OnCenterView()
{
    // Center the view !
    QMenu menu( this );
    menu.addAction( "Center the view" );
    menu.exec( QCursor::pos() );
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
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load actions file" ), defaultPath, tr( "Actions files (*.ord)" ) );
    if( !filename.IsEmpty() && filename.Exists() && filename.IsRegularFile() )
        emit LoadOrderFileRequest( filename );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnSaveOrderFile
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSaveOrderFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) );
    tools::Path filename = gui::FileDialog::getSaveFileName( this, tr( "Save actions in active timeline to file" ), defaultPath, tr( "Actions files (*.ord)" ) );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() != ".ord" )
        filename.ReplaceExtension( ".ord" );
    emit SaveOrderFileRequest( filename );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnLoadSessionFile
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineToolBar::OnLoadTimelineSessionFile()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load timeline session file" ), config_.BuildExerciseChildFile( config_.GetExerciseName() + "-session.timeline" ), tr( "Timeline session files (*.timeline)" ) );
    if( !filename.IsEmpty() && filename.Exists() && filename.IsRegularFile() )
        emit LoadTimelineSessionFileRequest( filename );
}

// -----------------------------------------------------------------------------
// Name: TimelineToolBar::OnSaveSessionFile
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineToolBar::OnSaveTimelineSessionFile()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-session" );
    tools::Path filename = gui::FileDialog::getSaveFileName( this, tr( "Save timeline session to file" ), defaultPath, tr( "Timeline session files (*.timeline)" ) );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() != ".timeline" )
        filename.ReplaceExtension( ".timeline" );
    emit SaveTimelineSessionFileRequest( filename );
}
