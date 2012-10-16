// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ExternalPerceptionCommand.h"
#include "RolePion_Perceiver.h"
#include "wrapper/View.h"
#include "wrapper/Event.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ExternalPerceptionCommand constructor
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
ExternalPerceptionCommand::ExternalPerceptionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExternalPerceptionCommand::Execute
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void ExternalPerceptionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    wrapper::Event event( "external perception" );
    const std::size_t perceiver = parameters[ "perceiver" ];
    const std::size_t target = parameters[ "target" ];
    event[ "entity/data" ] = model[ "entities" ][ perceiver ][ "data" ];
    event[ "level" ] = parameters[ "level" ];
    event[ "target/data" ] = model[ "entities" ][ target ][ "data" ];
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: ExternalPerceptionCommand::ExecutePaused
// Created: SLI 2012-03-16
// -----------------------------------------------------------------------------
void ExternalPerceptionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
