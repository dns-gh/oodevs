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
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/commands.h"
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtabwidget.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: JoinExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercisePanel::JoinExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "JoinExercisePanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Vertical, action.text(), box );
    {
        new QLabel( tr( "Choose the exercise to join:" ), group );
        list_ = new Q3ListBox( group );
    }
    {
        new QLabel( tr( "Choose the session to join:" ), group );
        sessionList_ = new Q3ListBox( group );
    }

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    Q3HBox* btnBox = new Q3HBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );

    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( sessionList_, SIGNAL( doubleClicked( Q3ListBoxItem* ) ), SLOT( StartExercise() ) );
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
        Start( new ::JoinExercise( config_, list_->selectedItem()->text(), sessionList_->selectedItem()->text() ) );
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
    if( Q3ListBoxItem* item = list_->selectedItem() )
    {
        sessionList_->insertStringList( commands::ListSessions( config_, item->text().ascii() ) );
        context_.Load( "session", sessionList_ );
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
    context_.Load( "exercise", list_ );
    ExerciseSelected();
}
