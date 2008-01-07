// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "JoinAnalysisPanel.h"
#include "moc_JoinAnalysisPanel.cpp"
#include "commands.h"
#include "JoinAnalysis.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qspinbox.h>

// -----------------------------------------------------------------------------
// Name: JoinAnalysisPanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysisPanel::JoinAnalysisPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 3, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the analysis to join:" ), group );
    list_ = new QListBox( group );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );

    QHBox* exerciseNumberBox = new QHBox( group );
    new QLabel( tr( "Exercise number:" ), exerciseNumberBox );
    exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox );

    Update();
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysisPanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysisPanel::~JoinAnalysisPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysisPanel::StartExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void JoinAnalysisPanel::StartExercise()
{
    if( list_->selectedItem() )
        new ::JoinAnalysis( this, config_, list_->selectedItem()->text(), exerciseNumber_->value() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysisPanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void JoinAnalysisPanel::Update()
{
    list_->clear();
    list_->insertStringList( commands::ListExercises( config_ ) );
    list_->setSelected( 0, true );
}
