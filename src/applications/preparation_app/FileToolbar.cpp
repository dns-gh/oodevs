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
#include "Menu.h"

namespace
{
    void AddToolButton( QToolBar* parent, QAction* action )
    {
        QToolButton* button = new QToolButton( parent );
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
    AddToolButton( this, menu.GetNewAction() );
    AddToolButton( this, menu.GetOpenAction() );
    AddToolButton( this, menu.GetSaveAction() );
    AddToolButton( this, menu.GetSaveAsAction() );
}
