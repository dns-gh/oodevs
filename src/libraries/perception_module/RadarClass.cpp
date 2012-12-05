// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RadarClass.h"

using namespace sword::perception;

RadarClass                  RadarClass::radar_       ( "radar"        , eRadar       , true  ); // Radar actif
RadarClass                  RadarClass::tapping_     ( "tapping"      , eTapping     , false ); // Radar passif
RadarClass                  RadarClass::tappingRadar_( "tapping-radar", eTappingRadar, false ); // Radar passif
RadarClass::T_RadarClassMap RadarClass::radarClasses_;

// -----------------------------------------------------------------------------
// Name: RadarClass::Initialize
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RadarClass::Initialize()
{
    radarClasses_.insert( std::make_pair( radar_       .GetName(), &radar_        ) );
    radarClasses_.insert( std::make_pair( tapping_     .GetName(), &tapping_      ) );
    radarClasses_.insert( std::make_pair( tappingRadar_.GetName(), &tappingRadar_ ) );
}

// -----------------------------------------------------------------------------
// Name: RadarClass::Terminate
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RadarClass::Terminate()
{
    radarClasses_.clear();
}

// -----------------------------------------------------------------------------
// Name: RadarClass constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
RadarClass::RadarClass( const std::string& strName, E_Type nType, bool bIsActive )
    : strName_  ( strName )
    , nType_    ( nType )
    , bIsActive_( bIsActive )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RadarClass destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
RadarClass::~RadarClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RadarClass::FindRadarClass
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const RadarClass* RadarClass::Find( const std::string& strClass )
{
    CIT_RadarClassMap it = radarClasses_.find( strClass );
    return it == radarClasses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::Find
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RadarClass* RadarClass::Find( std::size_t nID )
{
    for( auto it = radarClasses_.begin(); it != radarClasses_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::GetID
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
unsigned int RadarClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::IsActive
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool RadarClass::IsActive() const
{
    return bIsActive_;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::GetName
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const std::string& RadarClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::GetNbrClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
std::size_t RadarClass::GetNbrClasses()
{
    return radarClasses_.size();
}

// -----------------------------------------------------------------------------
// Name: RadarClass::GetRadarClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RadarClass::T_RadarClassMap& RadarClass::GetRadarClasses()
{
    return radarClasses_;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::operator==
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool RadarClass::operator==( const RadarClass& rhs ) const
{
    return rhs.nType_ == nType_;
}

// -----------------------------------------------------------------------------
// Name: RadarClass::operator!=
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool RadarClass::operator!=( const RadarClass& rhs ) const
{
    return rhs.nType_ != nType_;
}
