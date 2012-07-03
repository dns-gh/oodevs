// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "TogglePerceptionCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
TogglePerceptionCommand::TogglePerceptionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_ ( parameters[ "identifier" ] )
    , perception_ ( parameters[ "perception" ] )
    , isActivated_( parameters[ "activated" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
TogglePerceptionCommand::~TogglePerceptionCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void TogglePerceptionCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& perception = model[ "entities" ][ identifier_ ][ "perceptions"][ perception_ ][ "activated" ];
    if( perception == isActivated_ )
        return;
    wrapper::Effect effect( perception );
    effect = isActivated_;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: TogglePerceptionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void TogglePerceptionCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
