// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "StartAnalysePanel.h"
#include "moc_StartAnalysePanel.cpp"
#include "commands.h"
#include "StartReplay.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartAnalysePanel::StartAnalysePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
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
        new QLabel( tr( "Choose the replay to analyse:" ), replays );
        replays_ = new QListBox( replays );
        connect( replays_, SIGNAL( selectionChanged() ), SLOT( ReplaySelected() ) );
    }
    bubble_ = new InfoBubble( box );
    {
        QHBox* btnBox = new QHBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Start replay" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
    }
    
    connect( okay_, SIGNAL( pressed() ), SLOT( StartReplay() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartAnalysePanel::~StartAnalysePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel::ExerciseSelected
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysePanel::ExerciseSelected()
{
    replays_->clear();
    if( exercises_->selectedItem() )
    {
        QString exercise = exercises_->selectedItem()->text();
        replays_->insertStringList( commands::ListReplays( config_, exercise.ascii() ) );
        replays_->setSelected( 0, true );
        ReplaySelected();
    }
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel::ReplaySelected
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysePanel::ReplaySelected()
{
    const bool selected = replays_->selectedItem();
    okay_->setEnabled( selected );
    if( selected )
        bubble_->ShowInfo( tr( "Replay game: %1" ).arg( replays_->selectedItem()->text() ) ); // $$$$ SBO 2007-10-05: TODO
    else
        bubble_->ShowError( tr( "The selected exercise has no replay." ) ); // $$$$ SBO 2007-10-05: TODO
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel::StartReplay
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysePanel::StartReplay()
{
    if( exercises_->selectedItem() && replays_->selectedItem() )
        new ::StartReplay( this, config_, exercises_->selectedItem()->text(), replays_->selectedItem()->text() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void StartAnalysePanel::Update()
{
    exercises_->clear();
    exercises_->insertStringList( commands::ListExercises( config_ ) );
    exercises_->setSelected( 0, true );
}
