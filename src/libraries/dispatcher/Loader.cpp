// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Config.h"
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
Loader::Loader( ReplayModel_ABC& model, MessageHandler_ABC& handler, const Config& config, ClientPublisher_ABC& clients )
    : model_   ( model )
    , handler_ ( handler )
    , loader_  ( new MessageLoader( config.GetRecordDirectory(), false, config.GetTickDuration(), &clients ) )
    , keyFrame_( UINT_MAX )
    , frame_   ( UINT_MAX )
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
bool Loader::Start()
{
    unsigned int keyFrame = loader_->FindKeyFrame( 0 );
    if( keyFrame == UINT_MAX )
        return false;
    sword::SimToClient begin;
    begin.mutable_message()->mutable_control_send_current_state_begin();
    handler_.Receive( begin );
    SkipToFrame( keyFrame, 0 );
    sword::SimToClient end;
    end.mutable_message()->mutable_control_send_current_state_end();
    handler_.Receive( end );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Loader::SkipToFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::SkipToFrame( unsigned int frame )
{
    unsigned int keyFrame = loader_->FindKeyFrame( frame );
    if( keyFrame != UINT_MAX )
        SkipToFrame( keyFrame, frame );
}

void Loader::SkipToFrame( unsigned int keyFrame, unsigned int frame )
{
    const bool resync = keyFrame_ != keyFrame || frame < frame_;
    if( resync )
    {
        model_.StartSynchronisation();
        loader_->LoadKeyFrame( keyFrame, handler_ );
        keyFrame_ = keyFrame;
        frame_    = keyFrame;
    }
    while( frame_ <= frame && Tick() )
        continue;
    if( resync )
        model_.EndSynchronisation();
}

// -----------------------------------------------------------------------------
// Name: Loader::Tick
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
bool Loader::Tick()
{
    if( frame_ > GetTickNumber() )
        return false;
    const bool valid = loader_->LoadFrame( frame_, handler_ );
    if( valid )
        ++frame_;
    return valid;
}

// -----------------------------------------------------------------------------
// Name: Loader::GetCurrentTick
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Loader::GetCurrentTick() const
{
    return frame_;
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
// Name: Loader::GetFirstTick
// Created: JSR 2011-11-03
// -----------------------------------------------------------------------------
unsigned int Loader::GetFirstTick() const
{
    return loader_->GetFirstTick();
}

// -----------------------------------------------------------------------------
// Name: Loader::FillTimeTable
// Created: JSR 2011-07-25
// -----------------------------------------------------------------------------
void Loader::FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const
{
    loader_->FillTimeTable( msg, beginTick, endTick );
}

// -----------------------------------------------------------------------------
// Name: Loader::FindTickForDate
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
unsigned int Loader::FindTickForDate( const std::string& GDHDate ) const
{
    return loader_->FindTickForDate( GDHDate );
}

// -----------------------------------------------------------------------------
// Name: Loader::GetEndDateTime
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
const std::string& Loader::GetEndDateTime() const
{
    return loader_->GetEndDateTime();
}

// -----------------------------------------------------------------------------
// Name: Loader::ReloadAll
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void Loader::ReloadAll()
{
    loader_->ReloadAllFragmentsInfos();
}

void Loader::SendTimeskips( ClientPublisher_ABC& client ) const
{
    loader_->SendTimeskips( client );
}