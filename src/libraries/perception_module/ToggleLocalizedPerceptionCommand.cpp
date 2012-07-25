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

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedPerceptionCommand::ToggleLocalizedPerceptionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , isActivated_ ( parameters[ "activated" ] )
    , perception_  ( parameters[ "perception" ] )
    , perceptionId_( parameters[ "perception-id" ] )
    , localization_( isActivated_ ? parameters[ "localization" ].GetUserData() : 0 )
{
    // NOTHING
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
void ToggleLocalizedPerceptionCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& perception = model[ "entities" ][ identifier_ ][ "perceptions"][ perception_ ];
    wrapper::Effect effect( perception );
    if( isActivated_ )
        effect[ perceptionId_ ].SetUserData( localization_ );
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedPerceptionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedPerceptionCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
