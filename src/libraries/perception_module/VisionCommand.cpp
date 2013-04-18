// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "VisionCommand.h"
#include <tools/Exception.h>
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: VisionCommand constructor
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
VisionCommand::VisionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void VisionCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

namespace
{
    std::string CheckMode( const wrapper::View& mode )
    {
        const std::string result = mode;
        if( result == "normal" || result == "direction" || result == "location" )
            return result;
        throw MASA_EXCEPTION( "unknown vision mode '" + result + "'" );
    }
}

// -----------------------------------------------------------------------------
// Name: VisionCommand::Execute
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void VisionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const size_t identifier = parameters[ "identifier" ];
    const std::string mode = CheckMode( parameters[ "mode" ] );
    const wrapper::View& entity = model[ "entities" ][ identifier ];
    wrapper::Effect effect( entity[ "perceptions/vision" ] );
    effect[ "mode" ] = mode;
    if( mode == "normal" )
        effect[ "location" ] = entity[ "movement/direction" ];
    else
        effect[ "location" ] = parameters[ "location" ];
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: VisionCommand::ExecutePaused
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void VisionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}