// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FloodCommand.h"
#include "FloodModel.h"
#include "wrapper/Event.h"
#include "wrapper/View.h"
#include <geometry/Types.h>

using namespace sword;
using namespace sword::propagation;

// -----------------------------------------------------------------------------
// Name: FloodCommand constructor
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
FloodCommand::FloodCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void FloodCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCommand::Execute
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
void FloodCommand::Execute( const wrapper::View& parameters, const wrapper::View& /*model*/ ) const
{
    wrapper::Event flood( "flood event" );
    flood[ "identifier" ] = parameters[ "identifier" ];
    const geometry::Point2f center( parameters[ "center/x" ], parameters[ "center/y" ] );
    FloodModel().Generate( center, flood, parameters[ "depth" ], parameters[ "radius" ] );
    flood.Post();
}

// -----------------------------------------------------------------------------
// Name: FloodCommand::ExecutePaused
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
void FloodCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
