// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ModalDialog.h"

// -----------------------------------------------------------------------------
// Name: ModalDialog constructor
// Created: JSR 2012-03-20
// -----------------------------------------------------------------------------
ModalDialog::ModalDialog( QWidget *parent, const char *name, bool modal /* = true */, Qt::WindowFlags f /*= 0*/)
    : QDialog( parent, name, modal, f )
{
    QShortcut* s = new QShortcut( QKeySequence( Qt::Key_F1 ), this );
    if( parent )
        connect( s, SIGNAL( activated() ), parent, SIGNAL( ShowHelp() ) );
}

// -----------------------------------------------------------------------------
// Name: ModalDialog destructor
// Created: JSR 2012-03-20
// -----------------------------------------------------------------------------
ModalDialog::~ModalDialog()
{
    // NOTHING
}
