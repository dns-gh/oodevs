// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "MainMenu.h"
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qaction.h>

// -----------------------------------------------------------------------------
// Name: MainMenu constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
MainMenu::MainMenu( QMainWindow* mainWindow )
    : QObject( mainWindow )
    , mainWindow_( mainWindow )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MainMenu destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
MainMenu::~MainMenu()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MainMenu::AddAction
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void MainMenu::AddAction( const QString& category, QAction& action )
{
    QPopupMenu*& menu = menus_[ category ];
    if( !menu )
    {
        menu = new QPopupMenu( mainWindow_ );
        mainWindow_->menuBar()->insertItem( category, menu );
    }
    action.addTo( menu );
}
