// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "BreachableSegmentStruct.h"

using namespace rpr;

LinearSegmentStruct::LinearSegmentStruct()
    : segmentNumber_( 0 )
    , percentComplete_( 0 )
    , length_(0 )
    , width_(0 )
    , height_(0 )
    , depth_(0 )
    , damagedState_( damageState_NoDamage )
    , deactivated_( false )
    , flaming_( false )
    , predistributed_( false )
    , smoking_( false )
{
    // NOTHING
}

LinearSegmentStruct::~LinearSegmentStruct()
{
    // NOTHING
}

BreachableSegmentStruct::BreachableSegmentStruct()
    : breachedLength_( 0 )
    , breachedState_( breachedStatus_NoBreaching )
{
    // NOTHING
}

BreachableSegmentStruct::~BreachableSegmentStruct()
{
    // NOTHING
}
