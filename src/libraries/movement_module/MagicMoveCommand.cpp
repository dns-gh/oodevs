// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MagicMoveCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::movement;

// -----------------------------------------------------------------------------
// Name: MagicMoveCommand constructor
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
MagicMoveCommand::MagicMoveCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicMoveCommand::Execute
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
void MagicMoveCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const wrapper::View& entity = model[ "entities" ][ identifier ];
    wrapper::Effect effect( entity[ "movement" ] );
    effect[ "speed" ] = 0;
    effect[ "position" ] = parameters[ "position" ];
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: MagicMoveCommand::ExecutePaused
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
void MagicMoveCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
