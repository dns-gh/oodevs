// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "RestartExercisePanel.h"
#include "moc_RestartExercisePanel.cpp"
#include "commands.h"
#include "StartExercise.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qtimer.h>

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
RestartExercisePanel::RestartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : StartExercisePanel( widget, action, config )
{
    {
        new QLabel( tr( "Choose the session to restart:" ), listBox_ );
        sessionList_ = new QListBox( listBox_ );
    }
    {
        new QLabel( tr( "Choose the checkpoint to load:" ), listBox_ );
        checkpointList_ = new QListBox( listBox_ );
    }
    connect( sessionList_,    SIGNAL( selectionChanged() ), SLOT( SessionSelected() ) );
    connect( checkpointList_, SIGNAL( selectionChanged() ), SLOT( CheckpointSelected() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
RestartExercisePanel::~RestartExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::StartExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void RestartExercisePanel::StartExercise()
{
//    if( list_->selectedItem() )
//        new ::StartExercise( this, config_, list_->selectedItem()->text(),  ); // $$$$ AGE 2008-01-04: 
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::OnTimer
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void RestartExercisePanel::OnTimer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::ExerciseSelected
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void RestartExercisePanel::ExerciseSelected()
{
    sessionList_->clear();
    if( QListBoxItem* item = list_->selectedItem() )
    {
        sessionList_->insertStringList( commands::ListSessions( config_, item->text().ascii() ) );
        sessionList_->setSelected( 0, true );
    }
    SessionSelected();
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::SessionSelected
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void RestartExercisePanel::SessionSelected()
{
    checkpointList_->clear();
    if( QListBoxItem* item = sessionList_->selectedItem() )
    {
        checkpointList_->insertStringList( commands::ListCheckpoints( config_, list_->selectedItem()->text().ascii(), item->text().ascii() ) );
        checkpointList_->setSelected( 0, true );
    }
    CheckpointSelected();
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::CheckpointSelected
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void RestartExercisePanel::CheckpointSelected()
{
    // $$$$ AGE 2007-10-08: read meta data
    const bool selected = checkpointList_->selectedItem();
    okay_->setEnabled( selected );
    if( selected )
        bubble_->ShowInfo( tr( "Restart game: %1" ).arg( checkpointList_->selectedItem()->text() ) ); // $$$$ SBO 2007-10-05: TODO
    else
        bubble_->ShowError( tr( "The selected exercise as no checkpoint to restart." ) ); // $$$$ SBO 2007-10-05: TODO
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void RestartExercisePanel::Update()
{
    list_->clear();
    list_->insertStringList( commands::ListExercises( config_ ) );
    list_->setSelected( 0, true );
}
