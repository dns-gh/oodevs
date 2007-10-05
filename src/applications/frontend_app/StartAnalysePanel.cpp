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
    QHBox* box = new QHBox( this );
    box->setMargin( 10 );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        QVBox* exercises = new QVBox( group );
        new QLabel( tr( "Choose the exercise to analyse:" ), exercises );
        exercises_ = new QListBox( exercises );
        exercises_->insertStringList( commands::ListExercises( config ) );
        connect( exercises_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );
    }
    {
        QVBox* replays = new QVBox( group );
        new QLabel( tr( "Choose the replay to analyse:" ), replays );
        replays_ = new QListBox( replays );
    }
    exercises_->setSelected( 0, true );
    okay_ = new QPushButton( tr( "Ok" ), this );
    connect( okay_, SIGNAL( pressed() ), SLOT( StartReplay() ) );
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
    }
}

// -----------------------------------------------------------------------------
// Name: StartAnalysePanel::StartReplay
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysePanel::StartReplay()
{
    if( exercises_->selectedItem() && replays_->selectedItem() )
        new ::StartReplay( this, config_, exercises_->selectedItem()->text(), replays_->selectedItem()->text() );
}
