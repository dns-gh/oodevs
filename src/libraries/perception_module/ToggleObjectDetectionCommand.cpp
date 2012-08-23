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
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleObjectDetectionCommand::ToggleObjectDetectionCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , isActivated_ ( parameters[ "activated" ] )
    , speed_       ( isActivated_ ? parameters[ "growth-speed" ] : 0. )
    , centerX_     ( isActivated_ ? parameters[ "center/x" ] : 0. )
    , centerY_     ( isActivated_ ? parameters[ "center/y" ] : 0. )
    , perceptionId_( parameters[ "perception-id" ] )
    , localization_( isActivated_ ? static_cast< TER_Localisation* >( parameters[ "localization" ].GetUserData() ) : 0 )
{
    // NOTHING
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
void ToggleObjectDetectionCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& radar = model[ "entities" ][ identifier_ ][ "perceptions/object-detection"];
    wrapper::Effect effect( radar );
    if( isActivated_ )
    {
        effect[ perceptionId_ ][ "growth-speed" ] = speed_;
        effect[ perceptionId_ ][ "center/x" ] = centerX_;
        effect[ perceptionId_ ][ "center/y" ] = centerY_;
        effect[ perceptionId_ ][ "radius" ] = 0;
        effect[ perceptionId_ ][ "max-radius-reached" ] = false;
        effect[ perceptionId_ ][ "identifier" ] = perceptionId_;
        effect[ perceptionId_ ][ "localization" ].SetUserData( localization_ );
    }
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleObjectDetectionCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleObjectDetectionCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
