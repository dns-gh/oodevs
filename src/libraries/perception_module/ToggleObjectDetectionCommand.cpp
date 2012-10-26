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
#include "wrapper/Remove.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleObjectDetectionCommand::ToggleObjectDetectionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
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
    const std::size_t perceptionId = parameters[ "perception-id" ];
    const wrapper::View& perception = model[ "entities" ][ identifier ][ "perceptions/object-detection" ];
    if( parameters[ "activated" ] )
    {
        wrapper::Effect effect( perception );
        effect[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect[ perceptionId ][ "perception-id" ] = parameters[ "perception-id" ];
        effect[ perceptionId ][ "center" ] = parameters[ "center" ];
        effect[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect[ perceptionId ][ "radius" ] = 0;
        effect[ perceptionId ][ "max-radius-reached" ] = false;
        effect.Post();
    }
    else
        wrapper::Remove( perception[ perceptionId ] ).Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
