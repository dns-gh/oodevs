// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FileToolbar.h"
#include "clients_gui/RichWidget.h"
#include "Menu.h"

namespace
{
    void AddToolButton( const QString& objectName, QToolBar* parent, QAction* action )
    {
        gui::RichWidget< QToolButton >* button = new gui::RichWidget< QToolButton >( objectName, parent );
        button->setDefaultAction( action );
        parent->addWidget( button );
    }
}

// -----------------------------------------------------------------------------
// Name: FileToolbar constructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::FileToolbar( QWidget* parent, kernel::Controllers& controllers )
    : gui::RichToolBar( controllers, parent, "filetoolbar", tr( "Standard" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileToolbar destructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::~FileToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileToolbar::Fill
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void FileToolbar::Fill( Menu& menu )
{
    gui::SubObjectName subObject( "FileToolbar" );
    AddToolButton( "NewAction", this, menu.GetNewAction() );
    AddToolButton( "OpenAction", this, menu.GetOpenAction() );
    AddToolButton( "SaveAction", this, menu.GetSaveAction() );
    AddToolButton( "SaveAsAction", this, menu.GetSaveAsAction() );
}
