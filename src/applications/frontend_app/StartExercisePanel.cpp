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
#include "InfoBubble.h"
#include "GameConfigPanel.h"
#include "HlaPluginConfigPanel.h"
#include "resources.h"
#include "frontend/commands.h"
#include "frontend/StartExercise.h"
#include "tools/GeneralConfig.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qtimer.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

using namespace frontend;

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

    QGroupBox* group = new QGroupBox( 1, Qt::Horizontal, action.text(), box );
    QTabWidget* tabs = new QTabWidget( group );
    tabs->setMargin( 5 );
    
    listBox_ = new QGroupBox( 2, Qt::Vertical, box );
    list_ = new QListBox( listBox_ );
    tabs->addTab( listBox_, action.text() );

    configPanel_ = new GameConfigPanel( tabs, config );
    tabs->addTab( configPanel_, tr( "Options" ) );

    hlaConfigPanel_ = new HlaPluginConfigPanel( tabs, config );
    tabs->addTab( hlaConfigPanel_, tr( "HLA Export" ) );

    {
        QHBox* sessionBox = new QHBox( group );
        sessionBox->setStretchFactor( new QLabel( tr( "Session name:" ), sessionBox ), 1 );
        sessionBox->setStretchFactor( sessionName_ = new QLineEdit( sessionBox ), 2 );
    }
    {
        QHBox* commentBox = new QHBox( group );
        commentBox->setStretchFactor( new QLabel( tr( "Session comments:" ), commentBox ), 1 );
        commentBox->setStretchFactor( sessionComment_ = new QTextEdit( commentBox ), 2 );
        sessionComment_->setMaximumHeight( 40 );
    }

    {
        QHBox* exerciseNumberBox = new QHBox( group );
        exerciseNumberBox->setStretchFactor( new QLabel( tr( "Exercise number:" ), exerciseNumberBox ), 1 );
        exerciseNumberBox->setStretchFactor( exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox ), 2 );
    }

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );
    connect( okay_, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );

    {
        QTimer* timer = new QTimer( this );
        connect( timer, SIGNAL( timeout() ), SLOT( OnTimer() ) );
        timer->start( 1000 );
    }

    Update();
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
        configPanel_->Commit( list_->selectedItem()->text().ascii(), session_,
                              sessionName_->text().ascii(), sessionComment_->text().ascii(), 
                              exerciseNumber_->value() );
        hlaConfigPanel_->Commit( list_->selectedItem()->text().ascii(), session_ );
        new ::StartExercise( this, config_, list_->selectedItem()->text(), session_.c_str() );
        Update();
        ShowNext();
    }
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::ExerciseSelected
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void StartExercisePanel::ExerciseSelected()
{
    OnTimer();
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void StartExercisePanel::Update()
{
    list_->clear();
    list_->insertStringList( commands::ListExercises( config_ ) );
    list_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::OnTimer
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void StartExercisePanel::OnTimer()
{
    session_ = bpt::to_iso_string( bpt::second_clock::local_time() );
    if( list_->selectedItem() )
        bubble_->ShowInfo( tr( "The session will be created in directory '%1'" ).arg( BuildSessionDirectory() ) );
    else
        bubble_->ShowInfo( tr( "Select an exercise to start" ) );
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::BuildSessionDirectory
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
QString StartExercisePanel::BuildSessionDirectory()
{
    const QString exercise = list_->selectedItem()->text();
    if( exercise.isEmpty() )
        return exercise;
    return config_.BuildSessionDir( exercise.ascii(), session_ ).c_str();
}
