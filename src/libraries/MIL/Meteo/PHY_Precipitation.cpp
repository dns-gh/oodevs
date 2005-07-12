// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/PHY_Precipitation.cpp $
// $Author: Nld $
// $Modtime: 25/11/04 18:22 $
// $Revision: 2 $
// $Workfile: PHY_Precipitation.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Precipitation.h"

PHY_Precipitation::T_PrecipitationMap PHY_Precipitation::precipitations_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

PHY_Precipitation PHY_Precipitation::none_     ( "PasDePrecipitation", eNone     , EnumPrecipitationType::pas_de_precipitation );
PHY_Precipitation PHY_Precipitation::sandStorm_( "TempeteDeSable"    , eSandStorm, EnumPrecipitationType::tempete_sable        );
PHY_Precipitation PHY_Precipitation::fog_      ( "Brouillard"        , eFog      , EnumPrecipitationType::brouillard           );
PHY_Precipitation PHY_Precipitation::drizzle_  ( "Crachin"           , eDrizzle  , EnumPrecipitationType::crachin              );
PHY_Precipitation PHY_Precipitation::rain_     ( "Pluie"             , eRain     , EnumPrecipitationType::pluie                );
PHY_Precipitation PHY_Precipitation::snow_     ( "Neige"             , eSnow     , EnumPrecipitationType::neige                );
PHY_Precipitation PHY_Precipitation::smoke_    ( "Fumigene"          , eSmoke    , (ASN1T_EnumPrecipitationType)-1             );

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Precipitation::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing precipitations" );
    precipitations_[ none_     .GetName() ] = &none_;
    precipitations_[ sandStorm_.GetName() ] = &sandStorm_;
    precipitations_[ fog_      .GetName() ] = &fog_;
    precipitations_[ drizzle_  .GetName() ] = &drizzle_;
    precipitations_[ rain_     .GetName() ] = &rain_;
    precipitations_[ snow_     .GetName() ] = &snow_;
    precipitations_[ smoke_    .GetName() ] = &smoke_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Precipitation::Terminate()
{
    precipitations_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Precipitation::PHY_Precipitation( const std::string& strName, E_Precipitation nType, ASN1T_EnumPrecipitationType nAsnID )
    : strName_( strName )
    , nType_  ( nType   )
    , nAsnID_ ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Precipitation::~PHY_Precipitation()
{
}
