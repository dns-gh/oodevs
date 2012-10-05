// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleObjectDetectionCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleObjectDetectionCommand::ToggleObjectDetectionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "perceptions/object-detection" ] );
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect[ perceptionId ][ "perception-id" ] = parameters[ "perception-id" ];
        effect[ perceptionId ][ "center" ] = parameters[ "center" ];
        effect[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect[ perceptionId ][ "radius" ] = 0;
        effect[ perceptionId ][ "max-radius-reached" ] = false;
    }
    else
        effect[ perceptionId ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
