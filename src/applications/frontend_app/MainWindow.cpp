// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "MainWindow.h"
#include "ActionList.h"
#include "MainMenu.h"
#include "TerrainCreationPanel.h"
#include "StartExercisePanel.h"
#include "StartAnalysisPanel.h"
#include "CreateExercisePanel.h"
#include "EditExercisePanel.h"
#include "RestartExercisePanel.h"
#include "JoinExercisePanel.h"
#include "JoinAnalysisPanel.h"
#include "Actions.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/resources.h"

#include <qapplication.h>
#include <qaction.h>
#include <qwidgetstack.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( new tools::GeneralConfig() )
{
    config_->Parse( qApp->argc(), qApp->argv() );
    setCaption( APP_NAME );
    setIcon( MAKE_PIXMAP( csword ) );
    setMinimumSize( 800, 600 );

    menu_ = new MainMenu( this );

    QHBox* box = new QHBox( this );
    list_ = new ActionList( box );
    box->setStretchFactor( list_, 1 );

    pages_ = new QWidgetStack( box );
    pages_->addWidget( new QWidget( this ) );
    box->setStretchFactor( pages_, 3 );

    Actions& actions = *new Actions( this );

    AddAction< TerrainCreationPanel >( tr( "Prepare" ), actions.CreateTerrain() );
    AddAction< CreateExercisePanel >( tr( "Prepare" ), actions.CreateExercise() );
    AddAction< EditExercisePanel >( tr( "Prepare" ), actions.EditExercise() );

    AddAction< StartExercisePanel >( tr( "Play" ), actions.StartExercise() );
    AddAction< RestartExercisePanel >( tr( "Play" ), actions.RestartExercise() );
    AddAction< JoinExercisePanel >( tr( "Play" ), actions.JoinExercise() );

    AddAction< StartAnalysisPanel >( tr( "Analyse" ), actions.StartAnalysis() );
    AddAction< JoinAnalysisPanel >( tr( "Analyse" ), actions.JoinAnalysis() );

    linker_, "Create exercise", "Edit exercise", "Start exercise", "Join exercise";
    linker_, "Start analysis", "Join analysis";

    statusBar();

    setCentralWidget( box );
    CenterWindow();
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CenterWindow
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void MainWindow::CenterWindow()
{
    const QRect& screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - QPoint( width() / 2, height() / 2 ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::AddAction
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
template< typename Page >
void MainWindow::AddAction( const QString& category, QAction& action )
{
    menu_->AddAction( category, action );
    list_->AddAction( category, action );
    Page* page = new Page( pages_, action, *config_ );
    linker_.Register( action.name(), *page );
    pages_->addWidget( page );
}
