// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_HumanRank.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"

PHY_HumanRank::T_HumanRankMap PHY_HumanRank::humanRanks_;

const PHY_HumanRank PHY_HumanRank::officier_       ( "Officier"       , eOfficier       , sword::officer    , true  );
const PHY_HumanRank PHY_HumanRank::sousOfficier_   ( "SousOfficier"   , eSousOfficier   , sword::sub_officer, true  );
const PHY_HumanRank PHY_HumanRank::militaireDuRang_( "MilitaireDuRang", eMilitaireDuRang, sword::trooper    , false );

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanRank::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing human ranks" );
    humanRanks_[ officier_.GetName()        ] = &officier_;
    humanRanks_[ sousOfficier_.GetName()    ] = &sousOfficier_;
    humanRanks_[ militaireDuRang_.GetName() ] = &militaireDuRang_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanRank::Terminate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanRank::PHY_HumanRank( const std::string& strName, E_Rank nRank, const sword::EnumHumanRank nAsnID, bool bIsCommander )
    : strName_     ( strName )
    , nRank_       ( nRank )
    , nAsnID_      ( nAsnID )
    , bIsCommander_( bIsCommander )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanRank::~PHY_HumanRank()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetHumanRanks
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const PHY_HumanRank::T_HumanRankMap& PHY_HumanRank::GetHumanRanks()
{
    return humanRanks_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const PHY_HumanRank* PHY_HumanRank::Find( const std::string& strName )
{
    CIT_HumanRankMap it = humanRanks_.find( strName );
    return it == humanRanks_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_HumanRank* PHY_HumanRank::Find( unsigned int nID )
{
    for( auto it = humanRanks_.begin(); it != humanRanks_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
const PHY_HumanRank* PHY_HumanRank::Find( sword::EnumHumanRank nAsnID )
{
    for( auto it = humanRanks_.begin(); it != humanRanks_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const std::string& PHY_HumanRank::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetAsnID
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
sword::EnumHumanRank PHY_HumanRank::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
unsigned int PHY_HumanRank::GetID() const
{
    return static_cast< unsigned int >( nRank_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::IsCommander
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
bool PHY_HumanRank::IsCommander() const
{
    return bIsCommander_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::operator==
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_HumanRank::operator==( const PHY_HumanRank& rhs ) const
{
    return nRank_ == rhs.nRank_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::operator!=
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_HumanRank::operator!=( const PHY_HumanRank& rhs ) const
{
    return nRank_ != rhs.nRank_;
}
