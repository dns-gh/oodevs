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
#include "StartAnalysePanel.h"
#include "ExerciseCreationPanel.h"
#include "EditExercisePanel.h"
#include "RestartExercisePanel.h"
#include "JoinExercisePanel.h"
#include "resources.h"
#include "tools/GeneralConfig.h"

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
    setMinimumSize( 800, 600 );

    menu_ = new MainMenu( this );

    QHBox* box = new QHBox( this );
    list_ = new ActionList( box );
    box->setStretchFactor( list_, 1 );

    pages_ = new QWidgetStack( box );
    pages_->addWidget( new QWidget( this ) );
    box->setStretchFactor( pages_, 3 );

    AddAction< TerrainCreationPanel >( tr( "Prepare" ), CreateAction( tr( "Create terrain" ), MAKE_PIXMAP( terrain_create ), "Create terrain" ) );
    AddAction< ExerciseCreationPanel >( tr( "Prepare" ), CreateAction( tr( "Create exercise" ), MAKE_PIXMAP( data_create )   , "Create exercise" ) );
    AddAction< EditExercisePanel >( tr( "Prepare" ), CreateAction( tr( "Edit exercise" )  , MAKE_PIXMAP( data_create )   , "Edit exercise" ) );

    AddAction< StartExercisePanel >( tr( "Play" ), CreateAction( tr( "Start game" )  , MAKE_PIXMAP( terrain_create ), "Start game" ) );
    AddAction< RestartExercisePanel >( tr( "Play" ), CreateAction( tr( "Restart game" ), MAKE_PIXMAP( terrain_create ), "Restart game" ) );
    AddAction< JoinExercisePanel >( tr( "Play" ), CreateAction( tr( "Join game" )   , MAKE_PIXMAP( terrain_create ), "Join game" ) );

    AddAction< StartAnalysePanel >( tr( "Analyse" ), CreateAction( tr( "Start analysis" ) , MAKE_PIXMAP( terrain_create ), "Start analysis" ) );
    AddAction< JoinExercisePanel >( tr( "Analyse" ), CreateAction( tr( "Join analysis" ), MAKE_PIXMAP( terrain_create ), "Join analysis" ) );

    linker_.Chain( "Create exercise" )( "Edit exercise" )( "Start game" )( "Join game" );
    linker_.Chain( "Start analysis" )( "Join analysis" );

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
// Name: MainWindow::CreateAction
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& MainWindow::CreateAction( const QString& name, const QPixmap& pixmap, const char* cname )
{
    QAction* result = new QAction( pixmap, name, 0, this, cname );
    result->setText( name );
    result->setToggleAction( true );
    return *result;
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
