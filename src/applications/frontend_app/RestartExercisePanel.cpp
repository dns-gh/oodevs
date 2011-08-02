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
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/commands.h"
#include "frontend/StartExercise.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3textedit.h>
#include <Qtcore/qtimer.h>
#pragma warning( pop )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
RestartExercisePanel::RestartExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : StartExercisePanel( widget, action, config, context, "RestartExercisePanel" )
{
    {
        new QLabel( tr( "Choose the session to restart:" ), listBox_ );
        sessionList_ = new Q3ListBox( listBox_ );
    }
    {
        new QLabel( tr( "Choose the checkpoint to load:" ), listBox_ );
        checkpointList_ = new Q3ListBox( listBox_ );
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
    if( list_->selectedItem() && sessionList_->selectedItem() && checkpointList_->selectedItem() )
        Start( new ::StartExercise( config_, list_->selectedItem()->text(), sessionList_->selectedItem()->text(), checkpointList_->selectedItem()->text(), false ) ) ;
    context_.Save( "exercise", list_ );
    context_.Save( "session", sessionList_ );
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
    if( Q3ListBoxItem* item = list_->selectedItem() )
    {
        sessionList_->insertStringList( commands::ListSessions( config_, item->text().ascii() ) );
        context_.Load( "session", sessionList_ );
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
    if( Q3ListBoxItem* item = sessionList_->selectedItem() )
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
    const bool selected = checkpointList_->selectedItem() != 0;
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
    context_.Load( "exercise", list_ );
}
