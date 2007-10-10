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

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
RestartExercisePanel::RestartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the exercise to start:" ), group );
    list_ = new QListBox( group );
    list_->insertStringList( commands::ListExercises( config ) );

    new QLabel( tr( "Choose the checkpoint to load:" ), group );
    checkpointList_ = new QListBox( group );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Start exercise" ), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );
    connect( okay_, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( checkpointList_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );
    connect( checkpointList_, SIGNAL( selectionChanged() ), SLOT( CheckpointSelected() ) );
    list_->setSelected( 0, true );
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
    if( list_->selectedItem() )
        new ::StartExercise( this, config_, list_->selectedItem()->text() );
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: RestartExercisePanel::ExerciseSelected
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void RestartExercisePanel::ExerciseSelected()
{
    checkpointList_->clear();
    checkpointList_->insertStringList( commands::ListCheckpoints( config_, list_->selectedItem()->text().ascii() ) );
    checkpointList_->setSelected( 0, true );
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
