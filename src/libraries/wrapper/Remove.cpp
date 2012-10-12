// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Remove.h"
#include "View.h"
#include <module_api/Effect.h>

using namespace sword::wrapper;

// -----------------------------------------------------------------------------
// Name: Remove constructor
// Created: MCO 2012-10-12
// -----------------------------------------------------------------------------
Remove::Remove( const View& target )
    :target_( target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Remove::Post
// Created: MCO 2012-10-12
// -----------------------------------------------------------------------------
void Remove::Post() const
{
    ::SWORD_PostRemove( target_ );
}
