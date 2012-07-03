// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "VisionCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

namespace
{
    std::string CheckMode( const wrapper::View& mode )
    {
        const std::string result = mode;
        if( result == "normal" || result == "direction" || result == "location" )
            return result;
        throw std::runtime_error( "unknown vision mode '" + result + "'" );
    }
}

// -----------------------------------------------------------------------------
// Name: VisionCommand constructor
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
VisionCommand::VisionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_( parameters[ "identifier" ] )
    , mode_      ( CheckMode( parameters[ "mode" ] ) )
    , x_         ( mode_ == "normal" ? 0. : parameters[ "location/x" ] )
    , y_         ( mode_ == "normal" ? 1. : parameters[ "location/y" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionCommand destructor
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
VisionCommand::~VisionCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionCommand::Execute
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void VisionCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    wrapper::Effect effect( entity[ "perceptions/vision" ] );
    effect[ "mode" ] = mode_;
    effect[ "location/x" ] = mode_ == "normal" ? entity[ "movement/direction/x" ] : x_;
    effect[ "location/y" ] = mode_ == "normal" ? entity[ "movement/direction/y" ] : y_;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: VisionCommand::ExecutePaused
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void VisionCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
