// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsPanel.h"
#include "ActionsToolbar.h"
#include "ActionsListView.h"

// -----------------------------------------------------------------------------
// Name: ActionsPanel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsPanel::ActionsPanel( QMainWindow* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ActionsModel& actions, Publisher_ABC& publisher, const Simulation& simulation )
    : QDockWindow( parent )
{
    setResizeEnabled( true );
    setCaption( tr( "Actions" ) );
    setCloseMode( QDockWindow::Always );

    QVBox* box = new QVBox( this );
    setWidget( box );
    new ActionsToolbar( box, actions, controllers, publisher, simulation );
    new ActionsListView( box, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: ActionsPanel destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsPanel::~ActionsPanel()
{
    // NOTHING
}
