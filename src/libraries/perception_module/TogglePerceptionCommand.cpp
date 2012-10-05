// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "TogglePerceptionCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
TogglePerceptionCommand::TogglePerceptionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void TogglePerceptionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const size_t identifier = parameters[ "identifier" ];
    const std::string perceptionId = parameters[ "perception" ];
    const bool isActivated = parameters[ "activated" ];
    const wrapper::View& perception = model[ "entities" ][ identifier ][ "perceptions" ][ perceptionId ][ "activated" ];
    if( perception == isActivated )
        return;
    wrapper::Effect effect( perception );
    effect = isActivated;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void TogglePerceptionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
