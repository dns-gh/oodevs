// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Effect.h"
#include "View.h"
#include <module_api/Effect.h>

using namespace sword::wrapper;

// -----------------------------------------------------------------------------
// Name: Effect constructor
// Created: MCO 2012-01-02
// -----------------------------------------------------------------------------
Effect::Effect( const View& target )
    : effect_( ::SWORD_CreateEffect() )
    , node_  ( effect_ )
    , target_( target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Effect destructor
// Created: MCO 2012-01-02
// -----------------------------------------------------------------------------
Effect::~Effect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Effect::Post
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
void Effect::Post() const
{
    ::SWORD_PostEffect( effect_, target_ );
}

// -----------------------------------------------------------------------------
// Name: Effect::AddElement
// Created: VPR 2012-01-31
// -----------------------------------------------------------------------------
Node Effect::AddElement()
{
    return node_.AddElement();
}
