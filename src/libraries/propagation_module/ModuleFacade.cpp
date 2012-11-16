// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ModuleFacade.h"
#include "FloodCommand.h"
#include "PropagationCommand.h"
#include "wrapper/Command.h"

using namespace sword;
using namespace sword::propagation;

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade( const wrapper::View& model )
    : Hooks( model )
{
    sword::wrapper::RegisterCommand< FloodCommand >( "flood command" );
    sword::wrapper::RegisterCommand< PropagationCommand >( "propagation command" );
}
