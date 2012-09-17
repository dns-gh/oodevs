// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleRecoCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecoCommand::ToggleRecoCommand( ModuleFacade& /*module*/, const wrapper::View& /*parameters*/, const wrapper::View& /*model*/, size_t /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    wrapper::Effect effect( model[ "entities" ][ identifier ][ "perceptions/reco" ] );
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect[ perceptionId ][ "perception-id" ] = perceptionId;
        effect[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect[ perceptionId ][ "has-growth-speed" ] = parameters[ "has-growth-speed" ];
        effect[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect[ perceptionId ][ "radius" ] = 0;
        effect[ perceptionId ][ "max-radius-reached" ] = false;
    }
    else
        effect[ perceptionId ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
