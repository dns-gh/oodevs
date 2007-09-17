// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Loader.h"
#include "ReplayModel_ABC.h"
#include "ClientPublisher_ABC.h"
#include "MessageLoader.h"
#include "MessageHandler_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::Loader( ReplayModel_ABC& model, MessageHandler_ABC& handler, const Config& config, const std::string& records )
    : model_       ( model )
    , handler_     ( handler )
    , loader_      ( new MessageLoader( config, records, false ) )
    , currentFrame_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::~Loader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader::RequiresKeyFrame
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
bool Loader::RequiresKeyFrame( unsigned frame )
{
    unsigned key = frame / 100;
    unsigned currentKey = currentFrame_ / 100;
    return !frame || key != currentKey || frame < currentFrame_;
}

// -----------------------------------------------------------------------------
// Name: Loader::Start
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Loader::Start()
{
    loader_->LoadKeyFrame( 0, handler_ );

    // simulation the end of the initialization
    ASN1T_MsgsSimToClient message;
    message.msg.t = T_MsgsSimToClient_msg_msg_control_send_current_state_end;
    handler_.Receive( message );
}

// -----------------------------------------------------------------------------
// Name: Loader::SkipToFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::SkipToFrame( unsigned frame )
{
    const bool requiresKeyFrame = RequiresKeyFrame( frame );
    if( requiresKeyFrame )
    {
        model_.StartSynchronisation();
        currentFrame_ = loader_->LoadKeyFrame( frame, handler_ );
    }
    while( currentFrame_+1 < frame && Tick() )
        ;
    if( requiresKeyFrame )
        model_.EndSynchronisation();

    if( currentFrame_ < frame )
        Tick();
}

// -----------------------------------------------------------------------------
// Name: Loader::Tick
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
bool Loader::Tick()
{
    if( loader_->LoadFrame( currentFrame_, handler_ ) )
    {
        ++currentFrame_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Loader::GetCurrentTick
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Loader::GetCurrentTick() const
{
    return currentFrame_;
}

// -----------------------------------------------------------------------------
// Name: Loader::GetTickNumber
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Loader::GetTickNumber() const
{
    return loader_->GetTickNumber();
}
