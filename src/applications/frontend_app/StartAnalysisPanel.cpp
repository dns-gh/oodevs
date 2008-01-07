// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "StartAnalysisPanel.h"
#include "moc_StartAnalysisPanel.cpp"
#include "commands.h"
#include "StartReplay.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qspinbox.h>
#include <qpushbutton.h>

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartAnalysisPanel::StartAnalysisPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        QVBox* exercises = new QVBox( group );
        new QLabel( tr( "Choose the exercise to analyse:" ), exercises );
        exercises_ = new QListBox( exercises );
        connect( exercises_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );
    }
    {
        QVBox* replays = new QVBox( group );
        new QLabel( tr( "Choose the session to analyse:" ), replays );
        replays_ = new QListBox( replays );
        connect( replays_, SIGNAL( selectionChanged() ), SLOT( ReplaySelected() ) );
    }
    bubble_ = new InfoBubble( box );
    {
        QHBox* btnBox = new QHBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Start replay session" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
    }

    QHBox* exerciseNumberBox = new QHBox( group );
    new QLabel( tr( "Exercise number:" ), exerciseNumberBox );
    exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox );

    connect( okay_, SIGNAL( pressed() ), SLOT( StartReplay() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartAnalysisPanel::~StartAnalysisPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::ExerciseSelected
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysisPanel::ExerciseSelected()
{
    replays_->clear();
    if( exercises_->selectedItem() )
    {
        QString exercise = exercises_->selectedItem()->text();
        replays_->insertStringList( commands::ListSessions( config_, exercise.ascii() ) );
        replays_->setSelected( 0, true );
        ReplaySelected();
    }
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::ReplaySelected
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysisPanel::ReplaySelected()
{
    const bool selected = replays_->selectedItem();
    okay_->setEnabled( selected );
    if( selected )
        bubble_->ShowInfo( tr( "Replay session: %1" ).arg( replays_->selectedItem()->text() ) ); // $$$$ SBO 2007-10-05: TODO meta
    else
        bubble_->ShowError( tr( "The selected exercise has no session to replay." ) ); // $$$$ SBO 2007-10-05: TODO
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::StartReplay
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysisPanel::StartReplay()
{
    if( exercises_->selectedItem() && replays_->selectedItem() )
        new ::StartReplay( this, config_, exercises_->selectedItem()->text(), replays_->selectedItem()->text(), exerciseNumber_->value() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void StartAnalysisPanel::Update()
{
    exercises_->clear();
    exercises_->insertStringList( commands::ListExercises( config_ ) );
    exercises_->setSelected( 0, true );
}
