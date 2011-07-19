// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionLevel.cpp $
// $Author: Jvt $
// $Modtime: 16/03/05 19:04 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionLevel.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionLevel.h"
#include "protocol/Protocol.h"

const PHY_PerceptionLevel PHY_PerceptionLevel::identified_( "Identified", eIdentified, sword::UnitIdentification::identified );
const PHY_PerceptionLevel PHY_PerceptionLevel::recognized_( "Recognized", eRecognized, sword::UnitIdentification::recognized );
const PHY_PerceptionLevel PHY_PerceptionLevel::detected_  ( "Detected"  , eDetected  , sword::UnitIdentification::detected );
const PHY_PerceptionLevel PHY_PerceptionLevel::notSeen_   ( "NotSeen"   , eNotSeen   , sword::UnitIdentification::unseen );

PHY_PerceptionLevel::T_LevelMap PHY_PerceptionLevel::levels_;

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_PerceptionLevel::Initialize()
{
    levels_[ identified_.GetID() ] = &identified_;
    levels_[ recognized_.GetID() ] = &recognized_;
    levels_[ detected_  .GetID() ] = &detected_  ;
    levels_[ notSeen_   .GetID() ] = &notSeen_   ;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_PerceptionLevel::Terminate()
{
    levels_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_PerceptionLevel::PHY_PerceptionLevel( const std::string& strName, E_Level nLevel, const sword::UnitIdentification::Level& nAsnID )
    : strName_( strName )
    , nLevel_ ( nLevel  )
    , nAsnID_ ( nAsnID  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_PerceptionLevel::~PHY_PerceptionLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::FindPerceptionLevel
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionLevel::FindPerceptionLevel( unsigned int nID )
{
    CIT_LevelMap it = levels_.find( nID );

    return it != levels_.end() ? *it->second : notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::ConvertFromMsgIdToSimId
// Created: JSR 2011-07-19
// -----------------------------------------------------------------------------
const unsigned int PHY_PerceptionLevel::ConvertFromMsgIdToSimId( sword::UnitIdentification_Level level )
{
    switch( level )
    {
    case sword::UnitIdentification::identified :
        return identified_.GetID();
    case sword::UnitIdentification::recognized :
        return recognized_.GetID();
    case sword::UnitIdentification::detected :
        return detected_.GetID();
    case sword::UnitIdentification::unseen :
        return notSeen_.GetID();
    default:
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::IsBestLevel
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::IsBestLevel() const
{
    return nLevel_ == eIdentified;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const std::string& PHY_PerceptionLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
unsigned int PHY_PerceptionLevel::GetID() const
{
    return (unsigned int)nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::Serialize
// Created: SBO 2009-06-30
// -----------------------------------------------------------------------------
void PHY_PerceptionLevel::Serialize( sword::UnitIdentification::Level& level ) const
{
    level = nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator==
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator==( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ == rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator!=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ != rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator<
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator<( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ < rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator<=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator<=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ <= rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator>
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator>( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ > rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator>=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionLevel::operator>=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ >= rhs.nLevel_;
}
