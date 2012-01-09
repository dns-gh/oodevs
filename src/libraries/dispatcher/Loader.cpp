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
#include "MessageLoader.h"
#include "MessageHandler_ABC.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::Loader( ReplayModel_ABC& model, MessageHandler_ABC& handler, const Config& config )
    : model_          ( model )
    , handler_        ( handler )
    , loader_         ( new MessageLoader( config, false ) )
    , currentFrame_   ( 0 )
    , currentKeyFrame_( 0 )
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
// Name: Loader::Start
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Loader::Start()
{
    loader_->LoadKeyFrame( 0, handler_ );
    // simulates the end of the initialization
    sword::SimToClient wrapper;
    wrapper.mutable_message()->mutable_control_send_current_state_end();
    handler_.Receive( wrapper );
}

// -----------------------------------------------------------------------------
// Name: Loader::SkipToFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::SkipToFrame( unsigned int frame )
{
    unsigned int keyFrame = loader_->FindKeyFrame( frame );
    const bool requiresKeyFrame = keyFrame == 0  || currentKeyFrame_ != keyFrame || frame < currentFrame_;
    if( requiresKeyFrame )
    {
        model_.StartSynchronisation();
        loader_->LoadKeyFrame( keyFrame, handler_ );
        currentFrame_ = keyFrame;
        currentKeyFrame_ = keyFrame;
    }
    while( currentFrame_ + 1 < frame && Tick() )
        ;
    if( requiresKeyFrame )
        model_.EndSynchronisation();
    if( currentFrame_ < frame || frame == 0 )
        Tick();
}

// -----------------------------------------------------------------------------
// Name: Loader::Tick
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
bool Loader::Tick()
{
    if( currentFrame_ > GetTickNumber() )
        return false;
    bool ret = false;
    bool needSync = currentFrame_ == 0 && !model_.IsSynching();
    if( needSync )
        model_.StartSynchronisation();
    if( loader_->LoadFrame( currentFrame_, handler_ ) )
    {
        ++currentFrame_;
        ret = true;
    }
    if( needSync )
        model_.EndSynchronisation();
    return ret;
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

// -----------------------------------------------------------------------------
// Name: Loader::FillTimeTable
// Created: JSR 2011-07-25
// -----------------------------------------------------------------------------
void Loader::FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const
{
    loader_->FillTimeTable( msg, beginTick, endTick );
}
