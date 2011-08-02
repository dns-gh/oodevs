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
#include "InstallPackagePanel.h"
#include "CreatePackagePanel.h"
#include "Actions.h"
#include "frontend/Config.h"
#include "clients_gui/resources.h"
#include "clients_gui/Tools.h"
#include "clients_gui/HelpSystem.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : Q3MainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( new frontend::Config() )
{
    config_->Parse( qApp->argc(), qApp->argv() );
    setCaption( tools::translate( "Application", "SWORD" ) );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    setMinimumSize( 800, 600 );

    menu_ = new MainMenu( this );

    Q3HBox* box = new Q3HBox( this );
    list_ = new ActionList( box );
    box->setStretchFactor( list_, 1 );

    pages_ = new Q3WidgetStack( box );
    pages_->addWidget( new QWidget( this ) );
    box->setStretchFactor( pages_, 3 );

    Actions& actions = *new Actions( this );

    AddAction< TerrainCreationPanel >( tools::translate( "MainWindow", "Prepare" ), actions.CreateTerrain() );
    AddAction< CreateExercisePanel >( tools::translate( "MainWindow", "Prepare" ), actions.CreateExercise() );
    AddAction< EditExercisePanel >( tools::translate( "MainWindow", "Prepare" ), actions.EditExercise() );

    AddAction< StartExercisePanel >( tools::translate( "MainWindow", "Play" ), actions.StartExercise() );
    AddAction< RestartExercisePanel >( tools::translate( "MainWindow", "Play" ), actions.RestartExercise() );
    AddAction< JoinExercisePanel >( tools::translate( "MainWindow", "Play" ), actions.JoinExercise() );

    AddAction< StartAnalysisPanel >( tools::translate( "MainWindow", "Analyse" ), actions.StartAnalysis() );
    AddAction< JoinAnalysisPanel >( tools::translate( "MainWindow", "Analyse" ), actions.JoinAnalysis() );

    Q3Action& installAction = actions.InstallPackage();
    AddAction< InstallPackagePanel >( tools::translate( "MainWindow", "Manage" ), installAction );
    AddAction< CreatePackagePanel >( tools::translate( "MainWindow", "Manage" ), actions.CreatePackage() );

    linker_, "Create exercise", "Edit exercise", "Start exercise", "Join exercise";
    linker_, "Start analysis", "Join analysis";

    statusBar();

    setCentralWidget( box );
    CenterWindow();

    if( !config_->GetPackageFile().empty() )
        QTimer::singleShot( 0, &installAction, SLOT( toggle() ) );

    new gui::HelpSystem( this, config_->BuildResourceChildFile( "help/frontend.xml" ) );
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
void MainWindow::AddAction( const QString& category, Q3Action& action )
{
    menu_->AddAction( category, action );
    list_->AddAction( category, action );
    Page* page = new Page( pages_, action, *config_, context_ );
    linker_.Register( action.name(), *page );
    pages_->addWidget( page );
}
