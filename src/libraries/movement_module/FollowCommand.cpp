// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FollowCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::movement;

namespace
{
    void PostMovementIntention( const wrapper::View& parameters, const wrapper::View& model, bool intention )
    {
        const std::size_t identifier = parameters[ "identifier" ];
        wrapper::Effect effect( model[ "entities" ][ identifier ][ "movement/intention" ] );
        effect = intention;
        effect.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: FollowCommand constructor
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
FollowCommand::FollowCommand( const wrapper::View& parameters, const wrapper::View& model )
{
    PostMovementIntention( parameters, model, true );
}

// -----------------------------------------------------------------------------
// Name: FollowCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void FollowCommand::Destroy( const wrapper::View& parameters, const wrapper::View& model ) const
{
    PostMovementIntention( parameters, model, false );
}

// -----------------------------------------------------------------------------
// Name: FollowCommand::Execute
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
void FollowCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const std::size_t followed = parameters[ "followed" ];
    const wrapper::View& followedEntity = model[ "entities" ][ followed ][ "movement" ];
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "movement" ] );
    effect[ "position" ] = followedEntity[ "position" ];
    effect[ "direction" ] = followedEntity[ "direction" ];
    effect[ "speed" ] = followedEntity[ "speed" ];
    effect[ "height" ] = followedEntity[ "height" ];
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: FollowCommand::ExecutePaused
// Created: SLI 2012-10-23
// -----------------------------------------------------------------------------
void FollowCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
