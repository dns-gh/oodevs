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
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/commands.h"
#include "frontend/StartReplay.h"
#include "frontend/CommandLineTools.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartAnalysisPanel::StartAnalysisPanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "StartAnalysisPanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        Q3VBox* exercises = new Q3VBox( group );
        new QLabel( tr( "Choose the exercise to analyse:" ), exercises );
        exercises_ = new Q3ListBox( exercises );
        connect( exercises_, SIGNAL( selectionChanged() ), SLOT( ExerciseSelected() ) );
    }
    {
        Q3VBox* replays = new Q3VBox( group );
        new QLabel( tr( "Choose the session to analyse:" ), replays );
        replays_ = new Q3ListBox( replays );
        connect( replays_, SIGNAL( selectionChanged() ), SLOT( ReplaySelected() ) );
    }
    bubble_ = new InfoBubble( box );
    {
        Q3HBox* btnBox = new Q3HBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Start replay session" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
    }

    Q3HBox* exerciseNumberBox = new Q3HBox( group );
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
        replays_->insertStringList( frontend::commands::ListSessions( config_, exercise.ascii() ) );
        context_.Load( "session", replays_ );
        ReplaySelected();
    }
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::ReplaySelected
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysisPanel::ReplaySelected()
{
    const bool selected = replays_->selectedItem() != 0;
    okay_->setEnabled( selected );
    if( selected )
        bubble_->ShowInfo( BuildMessage( replays_->selectedItem()->text() ) );
    else
        bubble_->ShowError( tr( "The selected exercise has no session to replay." ) );
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::BuildMessage
// Created: AGE 2008-01-07
// -----------------------------------------------------------------------------
QString StartAnalysisPanel::BuildMessage( const QString& session ) const
{
    const QString exercise = exercises_->selectedItem()->text();
    const std::string sessionXml = config_.BuildSessionDir( exercise.ascii(), session.ascii() ) + "/session.xml"; // $$$$ AGE 2008-01-07:

    std::string name( " --- " ), comment(  " --- " );
    try
    {
        xml::xifstream xis( sessionXml );
        xis >> xml::start( "session" )
                >> xml::start( "meta" )
                    >> xml::content( "name", name )
                    >> xml::content( "comment", comment );
    }
    catch( ... )
    {
    }

    return tr( "Replay session: %1\n"
               "Name: %2\n"
               "Description: %3" ).arg( session ).arg( name.c_str() ).arg( comment.c_str() );
}

// -----------------------------------------------------------------------------
// Name: StartAnalysisPanel::StartReplay
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartAnalysisPanel::StartReplay()
{
    if( exercises_->selectedItem() && replays_->selectedItem() )
    {
        Start( new frontend::StartReplay( config_, exercises_->selectedItem()->text(), replays_->selectedItem()->text(), frontend::DispatcherPort( exerciseNumber_->value() ) ) );
        context_.Save( "exercise", exercises_ );
        context_.Save( "session", replays_ );
    }
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
    exercises_->insertStringList( frontend::commands::ListExercises( config_ ) );
    context_.Load( "exercise", exercises_ );
}
