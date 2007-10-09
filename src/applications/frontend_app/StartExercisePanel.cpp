// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "StartExercisePanel.h"
#include "moc_StartExercisePanel.cpp"
#include "commands.h"
#include "StartExercise.h"
#include "InfoBubble.h"
#include "GameConfigPanel.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qspinbox.h>

// -----------------------------------------------------------------------------
// Name: StartExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercisePanel::StartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Vertical, action.text(), box );
    QTabWidget* tabs = new QTabWidget( group );
    tabs->setMargin( 5 );
    
    list_ = new QListBox( tabs );
    list_->insertStringList( commands::ListExercises( config ) );
    tabs->addTab( list_, tr( "Exercise" ) );

    configPanel_ = new GameConfigPanel( tabs, config );
    tabs->addTab( configPanel_, tr( "Options" ) );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), tr( "Start exercise" ), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );

    QHBox* exerciseNumberBox = new QHBox( group );
    new QLabel( tr( "Exercise number:" ), exerciseNumberBox );
    exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox );

    list_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercisePanel::~StartExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::StartExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartExercisePanel::StartExercise()
{
    if( list_->selectedItem() )
    {
        configPanel_->Commit( list_->selectedItem()->text().ascii(), exerciseNumber_->value() );
        new ::StartExercise( this, config_, list_->selectedItem()->text() );
    }
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::ExerciseSelected
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
void StartExercisePanel::ExerciseSelected()
{
    if( list_->selectedItem() )
        configPanel_->Show( list_->selectedItem()->text().ascii() );
}
