// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Criteria.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Criteria constructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
Criteria::Criteria()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Criteria constructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
Criteria::Criteria( const std::string& value )
{
    criteria_.push_back( value );
}

// -----------------------------------------------------------------------------
// Name: Criteria destructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
Criteria::~Criteria()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Criteria::AddCriterion
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
void Criteria::AddCriterion( const std::string& value )
{
    criteria_.push_back( value );
}

// -----------------------------------------------------------------------------
// Name: Criteria::Matches
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
bool Criteria::Matches( const std::string& value ) const
{
    for( CIT_Criteria cit = criteria_.begin(); cit != criteria_.end(); ++cit )
        if( (*cit) == value )
            return true;
    return false;
}
