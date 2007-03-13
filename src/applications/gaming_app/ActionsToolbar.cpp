// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsToolbar.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent )
    : QHBox( parent )
{
    setMaximumHeight( 32 );
    QToolButton* btn = new QToolButton( this );
    btn->setPixmap( MAKE_PIXMAP( recplay ) );
    btn = new QToolButton( this );
    btn->setPixmap( MAKE_PIXMAP( recstop ) );
    btn = new QToolButton( this );
    btn->setPixmap( MAKE_PIXMAP( recrec ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::~ActionsToolbar()
{
    // NOTHING
}
