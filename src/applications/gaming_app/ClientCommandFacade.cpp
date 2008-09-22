// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ClientCommandFacade.h"
#include "SelectionCommand.h"
#include "gaming/CommandPublisher.h"

// -----------------------------------------------------------------------------
// Name: ClientCommandFacade constructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
ClientCommandFacade::ClientCommandFacade( QObject* parent, kernel::Controllers& controllers, Publisher_ABC& publisher )
    : QObject( parent )
    , publisher_( new CommandPublisher( controllers, publisher ) )
{
    new SelectionCommand( this, controllers, *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ClientCommandFacade destructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
ClientCommandFacade::~ClientCommandFacade()
{
    // NOTHING
}
