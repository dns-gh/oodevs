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

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecoCommand::ToggleRecoCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : effect_( model[ "entities" ][ static_cast< std::size_t >( parameters[ "identifier" ] ) ][ "perceptions/reco" ] )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect_[ perceptionId ][ "perception-id" ] = perceptionId;
        effect_[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect_[ perceptionId ][ "has-growth-speed" ] = parameters[ "has-growth-speed" ];
        effect_[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect_[ perceptionId ][ "radius" ] = 0;
        effect_[ perceptionId ][ "max-radius-reached" ] = false;
    }
    else
        effect_[ perceptionId ].MarkForRemove();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecoCommand::~ToggleRecoCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
