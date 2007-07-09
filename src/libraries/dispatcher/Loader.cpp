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
#include "Publisher_ABC.h"
#include "MessageLoader.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::Loader( ReplayModel_ABC& model, const Config& config, const std::string& records )
    : model_       ( model )
    , loader_      ( new MessageLoader( config, records ) )
    , currentFrame_( 0 )
{
    SkipToFrame( 0 );
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
// Name: Loader::SkipToFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::SkipToFrame( unsigned frame )
{
    const bool requiresKeyFrame = RequiresKeyFrame( frame );
    if( requiresKeyFrame )
    {
        model_.StartSynchronisation();
        loader_->LoadKeyFrame( frame, model_ );
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
    if( loader_->LoadFrame( currentFrame_, model_ ) )
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
