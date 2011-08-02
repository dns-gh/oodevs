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
#include "ActionsContext.h"
#include "GameConfigPanel.h"
#include "InfoBubble.h"
#include "resources.h"
#include "frontend/commands.h"
#include "frontend/EdxlHavePluginConfigPanel.h"
#include "frontend/PluginConfigBuilder.h"
#include "frontend/StartExercise.h"
#include "tools/GeneralConfig.h"
#include <Qt3Support/q3action.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtabwidget.h>
#include <Qt3Support/q3textedit.h>
#include <Qtcore/qtimer.h>
#include <boost/foreach.hpp>

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
StartExercisePanel::StartExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context, const char* name )
    : Panel_ABC( widget, action, context, name ? name : "StartExercisePanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 1, Qt::Horizontal, action.text(), box );
    QTabWidget* tabs = new QTabWidget( group );
    tabs->setMargin( 5 );

    listBox_ = new Q3GroupBox( 2, Qt::Vertical, box );
    list_ = new Q3ListBox( listBox_ );
    tabs->addTab( listBox_, action.text() );

    configPanel_ = new GameConfigPanel( tabs, config );
    tabs->addTab( configPanel_, tr( "Options" ) );

    {
        frontend::PluginConfigBuilder builder( config_, tabs );
        plugins_.push_back( builder.BuildFromXml()
                                   .Build< frontend::EdxlHavePluginConfigPanel >()
                                   .Finalize() );
    }
    {
        Q3HBox* sessionBox = new Q3HBox( group );
        sessionBox->setStretchFactor( new QLabel( tr( "Session name:" ), sessionBox ), 1 );
        sessionBox->setStretchFactor( sessionName_ = new QLineEdit( sessionBox ), 2 );
    }
    {
        Q3HBox* commentBox = new Q3HBox( group );
        commentBox->setStretchFactor( new QLabel( tr( "Session comments:" ), commentBox ), 1 );
        commentBox->setStretchFactor( sessionComment_ = new Q3TextEdit( commentBox ), 2 );
        sessionComment_->setMaximumHeight( 40 );
    }

    {
        Q3HBox* exerciseNumberBox = new Q3HBox( group );
        exerciseNumberBox->setStretchFactor( new QLabel( tr( "Exercise number:" ), exerciseNumberBox ), 1 );
        exerciseNumberBox->setStretchFactor( exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox ), 2 );
    }

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    Q3HBox* btnBox = new Q3HBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );
    connect( okay_, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( Q3ListBoxItem* ) ), SLOT( StartExercise() ) );
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
        const std::string session = session_;
        const QString exercise = list_->selectedItem()->text();
        configPanel_->Commit( exercise.ascii(), session,
                              sessionName_->text().ascii(), sessionComment_->text().ascii(),
                              exerciseNumber_->value() );
        BOOST_FOREACH( const T_Plugins::value_type& plugin, plugins_ )
            plugin->Commit( exercise.ascii(), session );
        Start( new ::StartExercise( config_, list_->selectedItem()->text(), session.c_str(), false ) ) ;
        context_.Save( "exercise", list_ );
        context_.Save( "session", session.c_str() );
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
    context_.Load( "exercise", list_ );
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

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::AddPlugin
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
template< typename T >
void StartExercisePanel::AddPlugin( QTabWidget* tabs )
{
    frontend::PluginConfig_ABC* plugin = new T( tabs, config_ );
    tabs->addTab( plugin, plugin->GetName() );
    plugins_.push_back( plugin );
}
