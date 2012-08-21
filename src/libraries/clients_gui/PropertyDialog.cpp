// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyDialog.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyDialog constructor
// Created: LGY 2012-08-16
// -----------------------------------------------------------------------------
PropertyDialog::PropertyDialog( QWidget* parent )
    : QDialog( parent )
{
    setModal( true );
}

// -----------------------------------------------------------------------------
// Name: PropertyDialog destructor
// Created: LGY 2012-08-16
// -----------------------------------------------------------------------------
PropertyDialog::~PropertyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDialog::showEvent
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
void PropertyDialog::showEvent( QShowEvent* event )
{
    QDialog::showEvent( event );
    QDesktopWidget* desktop = QApplication::desktop();
    QRect scr = desktop->screenGeometry( desktop->screenNumber( qApp->activeWindow() ) );
    move( scr.center() - rect().center() );
}
