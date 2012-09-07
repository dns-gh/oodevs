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
ToggleLocalizedPerceptionCommand::ToggleLocalizedPerceptionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : identifier_ ( parameters[ "identifier" ] )
    , perception_ ( parameters[ "perception" ] )
    , isActivated_( parameters[ "activated" ] )
    , effect_     ( model[ "entities" ][ identifier_ ][ "perceptions" ][ perception_ ] )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( isActivated_ )
        effect_[ perceptionId ][ "localization" ] = parameters[ "localization" ];
    else
        effect_[ perceptionId ].MarkForRemove();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedPerceptionCommand::~ToggleLocalizedPerceptionCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedPerceptionCommand::Execute( const wrapper::View& /*model*/ ) const
{
    if( !isActivated_ )
    {
        wrapper::Event event( perception_ + " disabled" );
        event[ "entity" ] = identifier_;
        event.Post();
    }
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedPerceptionCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
