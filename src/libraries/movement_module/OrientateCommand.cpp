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
OrientateCommand::OrientateCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_( parameters[ "identifier" ] )
    , x_         ( parameters[ "direction/x" ] )
    , y_         ( parameters[ "direction/y" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrientateCommand::Execute
// Created: MCO 2012-03-09
// -----------------------------------------------------------------------------
void OrientateCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    wrapper::Effect effect( model[ "entities" ][ identifier_ ][ "movement" ] );
    effect[ "direction/x" ] = x_;
    effect[ "direction/y" ] = y_;
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
