// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "SaverPlugin.h"
#include "SaverFacade.h"

using namespace plugins::saver;

// -----------------------------------------------------------------------------
// Name: SaverPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
SaverPlugin::SaverPlugin( dispatcher::ClientPublisher_ABC& client, dispatcher::Model_ABC& model, const dispatcher::Config& config )
    : saver_( new SaverFacade( client, model, config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
SaverPlugin::~SaverPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin::Receive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void SaverPlugin::Receive( const sword::SimToClient& message )
{
    saver_->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void SaverPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& , dispatcher::Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void SaverPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
