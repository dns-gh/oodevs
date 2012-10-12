// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleRecognitionPointCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Remove.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecognitionPointCommand::ToggleRecognitionPointCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecognitionPointCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const std::size_t perceptionId = parameters[ "perception-id" ];
    const wrapper::View& perception = model[ "entities" ][ identifier ][ "perceptions/recognition-point" ];
    if( parameters[ "activated" ] )
    {
        wrapper::Effect effect( perception );
        effect[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect[ perceptionId ][ "perception-id" ] = parameters[ "perception-id" ];
        effect[ perceptionId ][ "center" ] = parameters[ "center" ];
        effect[ perceptionId ][ "max-radius" ] = parameters[ "max-radius" ];
        effect[ perceptionId ][ "radius" ] = 0;
        effect[ perceptionId ][ "max-radius-reached" ] = false;
        effect.Post();
    }
    else
        wrapper::Remove( perception[ perceptionId ] ).Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecognitionPointCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
