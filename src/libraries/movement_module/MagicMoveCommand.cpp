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
// Name: MagicMoveCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void MagicMoveCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
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
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "movement" ] );
    effect[ "position" ] = parameters[ "position" ];
    effect[ "speed" ] = 0;
    effect[ "path/points" ];
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
