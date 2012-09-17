// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionCommand.h"
#include "RolePion_Perceiver.h"
#include "wrapper/View.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: PerceptionCommand constructor
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
PerceptionCommand::PerceptionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_( parameters[ "identifier" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCommand::Execute
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void PerceptionCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    RolePion_Perceiver().ExecutePerceptions( model, model[ "entities" ][ identifier_ ] );
}

// -----------------------------------------------------------------------------
// Name: PerceptionCommand::ExecutePaused
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void PerceptionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
