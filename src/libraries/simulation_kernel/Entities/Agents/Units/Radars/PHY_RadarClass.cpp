// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarClass.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:19 $
// $Revision: 1 $
// $Workfile: PHY_RadarClass.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RadarClass.h"

PHY_RadarClass                  PHY_RadarClass::radar_       ( "radar"       , eRadar       , true  ); // Radar actif
PHY_RadarClass                  PHY_RadarClass::tapping_     ( "ecoute"      , eTapping     , false ); // Radar passif
PHY_RadarClass                  PHY_RadarClass::tappingRadar_( "ecoute radar", eTappingRadar, false ); // Radar passif
PHY_RadarClass::T_RadarClassMap PHY_RadarClass::radarClasses_;

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::Initialize
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarClass::Initialize()
{
    radarClasses_.insert( std::make_pair( radar_       .GetName(), &radar_        ) );
    radarClasses_.insert( std::make_pair( tapping_     .GetName(), &tapping_      ) );
    radarClasses_.insert( std::make_pair( tappingRadar_.GetName(), &tappingRadar_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::Terminate
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarClass::Terminate()
{
    radarClasses_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_RadarClass::PHY_RadarClass( const std::string& strName, E_Type nType, bool bIsActive )
    : strName_  ( strName )
    , nType_    ( nType )
    , bIsActive_( bIsActive )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_RadarClass::~PHY_RadarClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::FindRadarClass
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const PHY_RadarClass* PHY_RadarClass::Find( const std::string& strClass )
{
    CIT_RadarClassMap it = radarClasses_.find( strClass );
    return it == radarClasses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::Find
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RadarClass* PHY_RadarClass::Find( unsigned int nID )
{
    for( auto it = radarClasses_.begin(); it != radarClasses_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetID
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
unsigned int PHY_RadarClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::IsActive
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_RadarClass::IsActive() const
{
    return bIsActive_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetName
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const std::string& PHY_RadarClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetNbrClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
std::size_t PHY_RadarClass::GetNbrClasses()
{
    return radarClasses_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetRadarClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RadarClass::T_RadarClassMap& PHY_RadarClass::GetRadarClasses()
{
    return radarClasses_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::operator==
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_RadarClass::operator==( const PHY_RadarClass& rhs ) const
{
    return rhs.nType_ == nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::operator!=
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_RadarClass::operator!=( const PHY_RadarClass& rhs ) const
{
    return rhs.nType_ != nType_;
}
