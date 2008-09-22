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
#include "HomePage.h"
#include "LinkInterpreter.h" 
#include "clients_gui/Tools.h"
#include "clients_gui/resources.h"
#include "frontend/Config.h"
#include "tools/Version.h"

#include <qapplication.h>
#include <qwidgetstack.h>
#include <qpixmap.h>
#include <qpalette.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::MainWindow( kernel::Controllers& controllers, boost::shared_ptr< SessionStatus > sessionStatus )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( new frontend::Config() )
    , interpreter_( new LinkInterpreter( this, controllers ) ) 
{

    config_->Parse( qApp->argc(), qApp->argv() );
    setCaption( tools::translate( "MainWindow", "SWORD Officer Training" ) + tools::translate( "MainWindow", " - release " ) + tools::AppVersion() );
    setIcon( MAKE_PIXMAP( csword ) );
    setFixedSize( 800, 600 );
    SetStyle();
    pages_ = new QWidgetStack( this );
    new HomePage( pages_, *config_, *interpreter_, sessionStatus );
    setCentralWidget( pages_ );
    CenterWindow();
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
