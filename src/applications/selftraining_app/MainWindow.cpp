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
#include "HomePage.h"
#include "clients_gui/resources.h"
#include "tools/GeneralConfig.h"
#include "graphics.cpp"
#include <qapplication.h>
#include <qwidgetstack.h>
#include <qpixmap.h>
#include <qpalette.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( new tools::GeneralConfig() )
{
    config_->Parse( qApp->argc(), qApp->argv() );
    setCaption( tr( "SWORD Officer Training" ) );
    setIcon( MAKE_PIXMAP( csword ) );
    setFixedSize( 800, 600 );

    SetStyle();

    pages_ = new QWidgetStack( this );
    new HomePage( pages_, *config_ );
    setCentralWidget( pages_ );
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
    QFont font( "Geneva", 10, QFont::Bold );
    setFont( font );

    QPalette p( palette() );
    p.setColor( QPalette::Active, QColorGroup::Background     , QColor( 48, 48, 64 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground     , Qt::white );
    p.setColor( QPalette::Active, QColorGroup::BrightText     , Qt::white );
    p.setColor( QPalette::Active, QColorGroup::Base           , QColor( 240, 240, 240 ) );
    p.setColor( QPalette::Active, QColorGroup::Text           , QColor(  32,  32,  48 ) );
    p.setColor( QPalette::Active, QColorGroup::ButtonText     , QColor(  32,  32,  48 ) );
    p.setColor( QPalette::Active, QColorGroup::Highlight      , QColor(  40, 135, 220 ) );
    p.setColor( QPalette::Active, QColorGroup::HighlightedText, Qt::white );
    p.setInactive( p.active() );
    p.setDisabled( p.active() );
    setPalette( p );
    setPaletteBackgroundPixmap( QPixmap( qembed_findData( "background2.jpg" ) ) );
}
