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
#include <qapplication.h>
#include <qwidgetstack.h>
#include <qpixmap.h>

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
    setMinimumSize( 800, 600 );

    setPaletteBackgroundColor( QColor( 64, 64, 80 ) );
    setPaletteForegroundColor( Qt::white );

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
