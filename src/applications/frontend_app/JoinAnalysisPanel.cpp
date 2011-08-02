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
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/commands.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/CommandLineTools.h"
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtabwidget.h>

// -----------------------------------------------------------------------------
// Name: JoinAnalysisPanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysisPanel::JoinAnalysisPanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "JoinAnalysisPanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 3, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the analysis to join:" ), group );
    list_ = new Q3ListBox( group );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    Q3HBox* btnBox = new Q3HBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( Q3ListBoxItem* ) ), SLOT( StartExercise() ) );

    Q3HBox* exerciseNumberBox = new Q3HBox( group );
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
    {
        Start( new frontend::JoinAnalysis( config_, list_->selectedItem()->text(), "", "", frontend::DispatcherPort( exerciseNumber_->value() ) ) );
        context_.Save( "exercise", list_ );
    }
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
    list_->insertStringList( frontend::commands::ListExercises( config_ ) );
    context_.Load( "exercise", list_ );
}
