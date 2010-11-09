// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "Config.h"
#include "HomePage.h"
#include "LinkInterpreter.h"
#include "MessageDialog.h"
#include "NetworkExerciseLister.h"
#include "clients_gui/resources.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Launcher.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"

#include <qapplication.h>
#include <qwidgetstack.h>
#include <qpixmap.h>
#include <qpalette.h>
#include <qtimer.h>

namespace
{
    Config* GetConfig()
    {
        Config* config = new Config();
        config->Parse( qApp->argc(), qApp->argv() );
        return config;
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::MainWindow( kernel::Controllers& controllers )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( GetConfig() )
    , interpreter_( new LinkInterpreter( this, controllers ) )
    , exerciseLister_( new NetworkExerciseLister( this, controllers, *config_ ) )
    , networkTimer_( new QTimer( this ) )
{
    setCaption( tools::translate( "Application", "SWORD" ) + tools::translate( "MainWindow", " - release " ) + tools::AppVersion() );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    setFixedWidth( 800 );
    setMinimumHeight( 600 );
    SetStyle();
    pages_ = new QWidgetStack( this );
    new HomePage( pages_, *config_, controllers, *exerciseLister_, *interpreter_ );
    setCentralWidget( pages_ );
    CenterWindow();

    try
    {
        launcher_.reset( new frontend::Launcher( controllers, *config_ ) );
        connect( networkTimer_, SIGNAL( timeout() ), SLOT( Update() ) );
        networkTimer_->start( 100 );
    }
    catch( std::exception& e )
    {
        MessageDialog message( this, tools::translate( "ProcessDialogs", "Failed to start launcher service" )
                                   , tools::translate( "ProcessDialogs", "%1.\nYou will not be able to start a multiplayer session. Do you want to start anyway?" ).arg( e.what() )
                                   , QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::No )
            throw;
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    networkTimer_->stop();
    launcher_.reset();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetStyle
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void MainWindow::SetStyle()
{
    QFont font( "Georgia", 12, QFont::Bold );
    setFont( font );
    QImage background( "resources/images/selftraining/background.jpg" );

    QPalette p( palette() );
    p.setColor( QPalette::Active, QColorGroup::Background     , QColor( 48, 48, 64 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground     , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::Active, QColorGroup::BrightText     , Qt::white );
    p.setBrush( QPalette::Active, QColorGroup::Base           , QBrush( QColor( 32, 32, 48 ), background ) );
    p.setColor( QPalette::Active, QColorGroup::Text           , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::Active, QColorGroup::Button         , QColor(   40, 40,  50 ) );
    p.setColor( QPalette::Active, QColorGroup::ButtonText     , QColor( 140, 140, 150 ) );
    p.setColor( QPalette::Active, QColorGroup::Highlight      , QColor(  40,  40,  40 ) );
    p.setColor( QPalette::Active, QColorGroup::HighlightedText, Qt::white );

    p.setColor( QPalette::Active, QColorGroup::Light   , QColor(  40,  40,  40 ) );
    p.setColor( QPalette::Active, QColorGroup::Midlight, QColor(  70, 170, 220 ) );
    p.setColor( QPalette::Active, QColorGroup::Dark    , QColor(  16, 130, 190 ) );
    p.setColor( QPalette::Active, QColorGroup::Mid     , QColor(  70, 170, 220 ) );
    p.setColor( QPalette::Active, QColorGroup::Shadow  , QColor(  40,  40,  40 ) );

    p.setInactive( p.active() );
    p.setDisabled( p.active() );
    setPalette( p );
    setPaletteBackgroundPixmap( background );
    setBackgroundOrigin( QWidget::WindowOrigin );
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
// Name: MainWindow::Maximize
// Created: RDS 2008-09-05
// -----------------------------------------------------------------------------
void MainWindow::Maximize()
{
    show();
    setActiveWindow();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Update
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void MainWindow::Update()
{
    if( launcher_.get() )
        launcher_->Update();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::resizeEvent
// Created: JSR 2010-07-16
// -----------------------------------------------------------------------------
void MainWindow::resizeEvent( QResizeEvent * )
{
    QImage background = QImage( "resources/images/selftraining/background.jpg" ).scale( width(), height() );
    QPalette p( palette() );
    p.setBrush( QPalette::Active, QColorGroup::Base, QBrush( QColor( 32, 32, 48 ), background ) );
    setPalette( p );
    setPaletteBackgroundPixmap( background );
}
