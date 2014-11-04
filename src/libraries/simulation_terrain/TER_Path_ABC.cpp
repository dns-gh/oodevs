// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Path_ABC.h"

namespace
{

unsigned int nIDIdx_ = 0;

} // namespace

// -----------------------------------------------------------------------------
// Name: TER_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_Path_ABC::TER_Path_ABC()
    : nID_( ++nIDIdx_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Path_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_Path_ABC::~TER_Path_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Path_ABC::GetID
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
unsigned int TER_Path_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: TER_Path_ABC::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool TER_Path_ABC::operator==( const TER_Path_ABC& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: TER_Path_ABC::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool TER_Path_ABC::operator!=( const TER_Path_ABC& rhs ) const
{
    return nID_ != rhs.nID_;
}
