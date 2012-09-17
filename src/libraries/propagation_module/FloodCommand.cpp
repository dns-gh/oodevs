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
#include <boost//foreach.hpp>

using namespace sword;
using namespace sword::propagation;

// -----------------------------------------------------------------------------
// Name: FloodCommand constructor
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
FloodCommand::FloodCommand( const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_( parameters[ "identifier" ] )
    , depth_     ( parameters[ "depth" ] )
    , radius_    ( parameters[ "radius" ] )
    , center_    ( parameters[ "center" ][ "x" ], parameters[ "center" ][ "y" ] )
    , model_     ( new FloodModel() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCommand destructor
// Created: LGY 2012-08-01
// -----------------------------------------------------------------------------
FloodCommand::~FloodCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCommand::Execute
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
void FloodCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    wrapper::Event flood( "flood event" );
    flood[ "identifier" ] = identifier_;
    model_->Generate( center_, flood, depth_, radius_ );
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
