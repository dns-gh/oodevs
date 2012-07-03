// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionLevel.h"

using namespace sword::perception;

const PerceptionLevel PerceptionLevel::identified_( "Identified", eIdentified );
const PerceptionLevel PerceptionLevel::recognized_( "Recognized", eRecognized );
const PerceptionLevel PerceptionLevel::detected_  ( "Detected"  , eDetected );
const PerceptionLevel PerceptionLevel::notSeen_   ( "NotSeen"   , eNotSeen );

PerceptionLevel::T_LevelMap PerceptionLevel::levels_;

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PerceptionLevel::Initialize()
{
    levels_[ identified_.GetID() ] = &identified_;
    levels_[ recognized_.GetID() ] = &recognized_;
    levels_[ detected_  .GetID() ] = &detected_  ;
    levels_[ notSeen_   .GetID() ] = &notSeen_   ;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PerceptionLevel::Terminate()
{
    levels_.clear();
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PerceptionLevel::PerceptionLevel( const std::string& strName, E_Level nLevel )
    : strName_( strName )
    , nLevel_ ( nLevel  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PerceptionLevel::~PerceptionLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::FindPerceptionLevel
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionLevel::FindPerceptionLevel( unsigned int nID )
{
    CIT_LevelMap it = levels_.find( nID );

    return it != levels_.end() ? *it->second : notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::IsBestLevel
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PerceptionLevel::IsBestLevel() const
{
    return nLevel_ == eIdentified;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const std::string& PerceptionLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
unsigned int PerceptionLevel::GetID() const
{
    return (unsigned int)nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator==
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator==( const PerceptionLevel& rhs ) const
{
    return nLevel_ == rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator!=( const PerceptionLevel& rhs ) const
{
    return nLevel_ != rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator<
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator<( const PerceptionLevel& rhs ) const
{
    return nLevel_ < rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator<=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator<=( const PerceptionLevel& rhs ) const
{
    return nLevel_ <= rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator>
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator>( const PerceptionLevel& rhs ) const
{
    return nLevel_ > rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionLevel::operator>=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PerceptionLevel::operator>=( const PerceptionLevel& rhs ) const
{
    return nLevel_ >= rhs.nLevel_;
}
