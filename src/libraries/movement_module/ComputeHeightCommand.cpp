    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ComputeHeightCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Hook.h"

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( ComputeHeight, double, ( const SWORD_Model* entity ) )

// -----------------------------------------------------------------------------
// Name: ComputeHeightCommand constructor
// Created: MCO 2012-03-12
// -----------------------------------------------------------------------------
ComputeHeightCommand::ComputeHeightCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComputeHeightCommand::Execute
// Created: MCO 2012-03-12
// -----------------------------------------------------------------------------
void ComputeHeightCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const unsigned int identifier = parameters[ "identifier" ];
    const wrapper::View& entity = model[ "entities" ][ identifier ];
    wrapper::Effect effect( entity[ "movement" ] );
    effect[ "height" ] = GET_HOOK( ComputeHeight )( entity );
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ComputeHeightCommand::ExecutePaused
// Created: MCO 2012-03-12
// -----------------------------------------------------------------------------
void ComputeHeightCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
