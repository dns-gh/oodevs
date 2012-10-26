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
PerceptionCommand::PerceptionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void PerceptionCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCommand::Execute
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void PerceptionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const unsigned int identifier = parameters[ "identifier" ];
    RolePion_Perceiver().ExecutePerceptions( model, model[ "entities" ][ identifier ] );
}

// -----------------------------------------------------------------------------
// Name: PerceptionCommand::ExecutePaused
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void PerceptionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
