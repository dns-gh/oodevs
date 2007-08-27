// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SaverPlugin.h"
#include "SaverFacade.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SaverPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
SaverPlugin::SaverPlugin( Model& model, const Config& config )
    : saver_( new SaverFacade( model, config ) )
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
void SaverPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    saver_->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void SaverPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& , Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void SaverPlugin::NotifyClientLeft( ClientPublisher_ABC& )
{
    // NOTHING
}
