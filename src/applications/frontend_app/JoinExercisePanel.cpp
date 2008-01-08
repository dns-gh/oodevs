// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "JoinExercisePanel.h"
#include "moc_JoinExercisePanel.cpp"
#include "commands.h"
#include "StartExercise.h"
#include "JoinExercise.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercisePanel::JoinExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Vertical, action.text(), box );
    {
        new QLabel( tr( "Choose the exercise to join:" ), group );
        list_ = new QListBox( group );
    }
    {
        new QLabel( tr( "Choose the session to join:" ), group );
        sessionList_ = new QListBox( group );
    }

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );

    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( sessionList_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );
    connect( sessionList_, SIGNAL( selectionChanged() ), SLOT( SessionSelected() ) );

    Update();
}

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercisePanel::~JoinExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel::StartExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void JoinExercisePanel::StartExercise()
{
    if( sessionList_->selectedItem() )
        new ::JoinExercise( this, config_, list_->selectedItem()->text(), sessionList_->selectedItem()->text() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel::ExerciseSelected
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void JoinExercisePanel::ExerciseSelected()
{
    sessionList_->clear();
    if( QListBoxItem* item = list_->selectedItem() )
    {
        sessionList_->insertStringList( commands::ListSessions( config_, item->text().ascii() ) );
        if( sessionList_->count() )
            sessionList_->setSelected( sessionList_->count()-1, true );
    }
    SessionSelected();
}

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel::SessionSelected
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void JoinExercisePanel::SessionSelected()
{
    // $$$$ AGE 2008-01-04: bubbule
}

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void JoinExercisePanel::Update()
{
    list_->clear();
    list_->insertStringList( commands::ListExercises( config_ ) );
    list_->setSelected( 0, true );
    ExerciseSelected();
}
