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

}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_RadarClass::~PHY_RadarClass()
{
}
