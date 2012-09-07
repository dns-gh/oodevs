// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleRecognitionPointCommand.h"
#include "wrapper/View.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecognitionPointCommand::ToggleRecognitionPointCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : effect_( model[ "entities" ][ static_cast< std::size_t >( parameters[ "identifier" ] ) ][ "perceptions/recognition-point" ] )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect_[ perceptionId ][ "growth-speed" ] = parameters[ "growth-speed" ];
        effect_[ perceptionId ][ "perception-id" ] = parameters[ "perception-id" ];
        effect_[ perceptionId ][ "center" ] = parameters[ "center" ];
        effect_[ perceptionId ][ "max-radius" ] = parameters[ "max-radius" ];
        effect_[ perceptionId ][ "radius" ] = 0;
        effect_[ perceptionId ][ "max-radius-reached" ] = false;
    }
    else
        effect_[ perceptionId ].MarkForRemove();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecognitionPointCommand::~ToggleRecognitionPointCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecognitionPointCommand::Execute( const wrapper::View& /*model*/ ) const
{
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecognitionPointCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
