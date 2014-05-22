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
#include "MessageDialog.h"
#include "SessionTray.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/resources.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/VersionHelper.h"

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Application& app, Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, ExerciseContainer& exercises )
    : gui::WidgetLanguageObserver_ABC< QMainWindow >()
    , sessionTray_( 0 )
{
    setWindowIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );
    setFixedWidth( 800 );
    setFixedHeight( 600 );
    SetStyle();
    pages_ = new QStackedWidget( this );
    HomePage* home = new HomePage( app, this, pages_, config, fileLoader, controllers, exercises );
    pages_->setCurrentWidget( home );
    setCentralWidget( pages_ );
    CenterWindow();
    if( !config.GetPackageFile().IsEmpty() )
        home->InstallPackage( config.GetPackageFile() );
    new gui::HelpSystem( this, tools::GeneralConfig::BuildResourceChildFile( "help/frontend.xml" ) );
    sessionTray_.reset( new SessionTray( this ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void MainWindow::OnLanguageChanged()
{
    setWindowTitle( tools::translate( "Application", "SWORD" ) + tools::translate( "MainWindow", " - release " ) + tools::AppProjectVersion() );
    if( sessionTray_.get() )
        sessionTray_->OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetStyle
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void MainWindow::SetStyle()
{
    QFont font( "Calibri", 12, QFont::Bold );
    setFont( font );
    QImage background( "resources/images/selftraining/background.png" );
    QPixmap px;
    px.convertFromImage( background );

    QPalette p( palette() );
    p.setColor( QPalette::Background     , QColor( 48, 48, 64 ) );
    p.setColor( QPalette::Foreground     , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::BrightText     , Qt::white );
    p.setBrush( QPalette::Base           , QBrush( QColor( 32, 32, 48 ), px ) );
    p.setColor( QPalette::Text           , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::Button         , QColor(   40, 40,  50 ) );
    p.setColor( QPalette::ButtonText     , QColor( 140, 140, 150 ) );
    p.setColor( QPalette::Highlight      , QColor(  40,  40,  40 ) );
    p.setColor( QPalette::HighlightedText, Qt::white );

    p.setColor( QPalette::Light   , QColor(  40,  40,  40 ) );
    p.setColor( QPalette::Midlight, QColor(  70, 170, 220 ) );
    p.setColor( QPalette::Dark    , QColor(  16, 130, 190 ) );
    p.setColor( QPalette::Mid     , QColor(  70, 170, 220 ) );
    p.setColor( QPalette::Shadow  , QColor(  40,  40,  40 ) );

    p.setBrush( backgroundRole(), QBrush( px ) );
    setPalette( p );
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
void MainWindow::Maximize( QSystemTrayIcon::ActivationReason reason )
{
    if( reason == QSystemTrayIcon::Trigger )
    {
        show();
        activateWindow();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::resizeEvent
// Created: JSR 2010-07-16
// -----------------------------------------------------------------------------
void MainWindow::resizeEvent( QResizeEvent * )
{
    QImage background = gui::Image( "resources/images/selftraining/background.png" ).scaledToHeight( height() );
    background = background.scaledToWidth( width() );
    QPalette p( palette() );
    QPixmap px;
    px.convertFromImage( background );
    p.setBrush( backgroundRole(), QBrush( px ) );
    setPalette( p );
}
