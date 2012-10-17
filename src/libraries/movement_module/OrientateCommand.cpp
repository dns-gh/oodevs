    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "OrientateCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::movement;

// -----------------------------------------------------------------------------
// Name: OrientateCommand constructor
// Created: MCO 2012-03-09
// -----------------------------------------------------------------------------
OrientateCommand::OrientateCommand( ModuleFacade& /*module*/, const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrientateCommand::Execute
// Created: MCO 2012-03-09
// -----------------------------------------------------------------------------
void OrientateCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const unsigned int identifier = parameters[ "identifier" ];
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "movement" ] );
    effect[ "direction" ] = parameters[ "direction" ];
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: OrientateCommand::ExecutePaused
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void OrientateCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
