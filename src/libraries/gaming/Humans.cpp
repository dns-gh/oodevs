// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Humans.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::Humans()
    : state_( eTroopHealthStateTotal )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::Humans( E_TroopHealthState state )
    : state_( state )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
Humans::Humans( const Humans& rhs )
{
    *this = rhs;
}

// -----------------------------------------------------------------------------
// Name: Humans destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::~Humans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans::operator=
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
const Humans& Humans::operator=( const Humans& rhs )
{
    state_       = rhs.state_;
    officers_    = rhs.officers_;
    subOfficers_ = rhs.subOfficers_;
    troopers_    = rhs.troopers_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Humans::operator-=
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
Humans& Humans::operator-=( const Humans& rhs )
{
    assert( state_ == rhs.state_ );
    officers_    -= rhs.officers_;
    subOfficers_ -= rhs.subOfficers_;
    troopers_    -= rhs.troopers_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Humans::operator-
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
Humans Humans::operator-( const Humans& rhs ) const
{
    Humans result( *this );
    result.officers_    -= rhs.officers_;
    result.subOfficers_ -= rhs.subOfficers_;
    result.troopers_    -= rhs.troopers_;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Humans::operator-
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
Humans Humans::operator-() const
{
    Humans result( *this );
    result.officers_    = -officers_;
    result.subOfficers_ = -subOfficers_;
    result.troopers_    = -troopers_;
    return result;
}
