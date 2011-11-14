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
#include "SessionTray.h"
#include "clients_gui/resources.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcherClient )
    : gui::LanguageChangeObserver_ABC< Q3MainWindow >()
    , interpreter_( new LinkInterpreter( this, controllers ) )
    , sessionTray_( 0 )
{
    setWindowFlags( Qt::WDestructiveClose );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    setFixedWidth( 800 );
    setFixedHeight( 600 );
    SetStyle();
    pages_ = new Q3WidgetStack( this );
    HomePage* home = new HomePage( this, pages_, config, fileLoader, controllers, launcherClient, *interpreter_ );
    setCentralWidget( pages_ );
    CenterWindow();
    if( !config.GetPackageFile().empty() )
        home->InstallPackage( config.GetPackageFile().c_str() );
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
    setCaption( tools::translate( "Application", "SWORD" ) + tools::translate( "MainWindow", " - release " ) + tools::AppVersion() );
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
    p.setColor( QPalette::Active, QColorGroup::Background     , QColor( 48, 48, 64 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground     , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::Active, QColorGroup::BrightText     , Qt::white );
    p.setBrush( QPalette::Active, QColorGroup::Base           , QBrush( QColor( 32, 32, 48 ), px ) );
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
void MainWindow::Maximize()
{
    show();
    setActiveWindow();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::resizeEvent
// Created: JSR 2010-07-16
// -----------------------------------------------------------------------------
void MainWindow::resizeEvent( QResizeEvent * )
{
    QImage background = QImage( "resources/images/selftraining/background.png" ).scaleHeight( height() );
    background = background.scaledToWidth( width() );
    QPalette p( palette() );
    QPixmap px;
    px.convertFromImage( background );
    p.setBrush( backgroundRole(), QBrush( px ) );
    setPalette( p );
}
