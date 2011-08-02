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
#include <Qt3Support/q3mainwindow.h>
#include <QtGui/qmenubar.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3action.h>

// -----------------------------------------------------------------------------
// Name: MainMenu constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
MainMenu::MainMenu( Q3MainWindow* mainWindow )
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
void MainMenu::AddAction( const QString& category, Q3Action& action )
{
    Q3PopupMenu*& menu = menus_[ category ];
    if( !menu )
    {
        menu = new Q3PopupMenu( mainWindow_ );
        mainWindow_->menuBar()->insertItem( category, menu );
    }
    action.addTo( menu );
}
