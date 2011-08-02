// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChatDock.h"
#include "ChatWidget.h"

// -----------------------------------------------------------------------------
// Name: ChatDock constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatDock::ChatDock( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler )
    : QDockWidget( "chat", parent )
{
    setObjectName( "chatDock" );
    setWidget( new ChatWidget( this, controllers, publisher, handler ) );
    setCaption( tr( "Chat" ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChatDock destructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatDock::~ChatDock()
{
    // NOTHING
}
