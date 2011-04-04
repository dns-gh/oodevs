// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "FrequencyCondition.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: FrequencyCondition constructor
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
FrequencyCondition::FrequencyCondition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FrequencyCondition destructor
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
FrequencyCondition::~FrequencyCondition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FrequencyCondition::Check
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
bool FrequencyCondition::Check( const unsigned int /*delta*/ )
{
    throw std::runtime_error( "to be implemented" );
}
