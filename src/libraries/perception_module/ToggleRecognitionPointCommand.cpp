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
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecognitionPointCommand::ToggleRecognitionPointCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , isActivated_ ( parameters[ "activated" ] )
    , size_        ( isActivated_ ? parameters[ "size" ] : 0. )
    , speed_       ( isActivated_ ? parameters[ "growth-speed" ] : 0. )
    , centerX_     ( isActivated_ ? parameters[ "center/x" ] : 0. )
    , centerY_     ( isActivated_ ? parameters[ "center/y" ] : 0. )
    , perceptionId_( parameters[ "perception-id" ] )
{
    // NOTHING
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
void ToggleRecognitionPointCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& point = model[ "entities" ][ identifier_ ][ "perceptions/recognition-point"];
    wrapper::Effect effect( point );
    if( isActivated_ )
    {
        effect[ perceptionId_ ][ "size" ] = size_;
        effect[ perceptionId_ ][ "growth-speed" ] = speed_;
        effect[ perceptionId_ ][ "center/x" ] = centerX_;
        effect[ perceptionId_ ][ "center/y" ] = centerY_;
    }
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecognitionPointCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecognitionPointCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
