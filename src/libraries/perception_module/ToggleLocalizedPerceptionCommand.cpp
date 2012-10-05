// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleLocalizedPerceptionCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedPerceptionCommand::ToggleLocalizedPerceptionCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedPerceptionCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const std::string perception = parameters[ "perception" ];
    const std::size_t perceptionId = parameters[ "perception-id" ];
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "perceptions" ][ perception ] );
    if( parameters[ "activated" ] )
        effect[ perceptionId ][ "localization" ] = parameters[ "localization" ];
    else
    {
        effect[ perceptionId ].MarkForRemove();
        wrapper::Event event( perception + " disabled" );
        event[ "entity" ] = identifier;
        event.Post();
    }
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedPerceptionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
