// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleObjectDetectionCommand.h"
#include "wrapper/View.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleObjectDetectionCommand::ToggleObjectDetectionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : effect_( model[ "entities" ][ static_cast< std::size_t >( parameters[ "identifier" ] ) ][ "perceptions/object-detection" ] )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect_[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect_[ perceptionId ][ "perception-id" ] = parameters[ "perception-id" ];
        effect_[ perceptionId ][ "center" ] = parameters[ "center" ];
        effect_[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect_[ perceptionId ][ "radius" ] = 0;
        effect_[ perceptionId ][ "max-radius-reached" ] = false;
    }
    else
        effect_[ perceptionId ].MarkForRemove();
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleObjectDetectionCommand::~ToggleObjectDetectionCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
