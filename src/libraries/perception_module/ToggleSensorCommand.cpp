// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleSensorCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

const char* const ToggleSensorCommand::name = "toggle sensor command";

// -----------------------------------------------------------------------------
// Name: ToggleSensorCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleSensorCommand::ToggleSensorCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_ ( parameters[ "identifier" ] )
    , isActivated_( parameters[ "activated" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleSensorCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleSensorCommand::~ToggleSensorCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleSensorCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleSensorCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& sensor = model[ "entities" ][ identifier_ ][ "perceptions/sensor/activated" ];
    if( sensor == isActivated_ )
        return;
    wrapper::Effect effect( sensor );
    effect = isActivated_;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleSensorCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleSensorCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
