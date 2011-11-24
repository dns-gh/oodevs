// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FilterViewCommand.h"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "gaming/UnitFilter.h"
#include <string>

// -----------------------------------------------------------------------------
// Name: FilterViewCommand constructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
FilterViewCommand::FilterViewCommand( CommandHandler& handler, UnitFilter& filter )
    : handler_( handler )
    , filter_ ( filter )
{
    handler_.Register( "clearfilter", *this );
}

// -----------------------------------------------------------------------------
// Name: FilterViewCommand destructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
FilterViewCommand::~FilterViewCommand()
{
    handler_.Unregister( "clearfilter", *this );
}

// -----------------------------------------------------------------------------
// Name: FilterViewCommand::Receive
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
void FilterViewCommand::Receive( const Command& command )
{
    if( command.Name() == "clearfilter" )
        filter_.RemoveFilter();
}
