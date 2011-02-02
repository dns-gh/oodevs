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

#include "PHY_Precipitation.h"

using namespace weather;

PHY_Precipitation::T_PrecipitationMap PHY_Precipitation::precipitations_;
PHY_Precipitation PHY_Precipitation::none_     ( "PasDePrecipitation", eWeatherType_None     , sword::WeatherAttributes::dry                       );
PHY_Precipitation PHY_Precipitation::sandStorm_( "TempeteDeSable"    , eWeatherType_SandStorm, sword::WeatherAttributes::sand_storm                );
PHY_Precipitation PHY_Precipitation::fog_      ( "Brouillard"        , eWeatherType_Fog      , sword::WeatherAttributes::fog                       );
PHY_Precipitation PHY_Precipitation::drizzle_  ( "Crachin"           , eWeatherType_Drizzle  , sword::WeatherAttributes::drizzle                   );
PHY_Precipitation PHY_Precipitation::rain_     ( "Pluie"             , eWeatherType_Rain     , sword::WeatherAttributes::rain                      );
PHY_Precipitation PHY_Precipitation::snow_     ( "Neige"             , eWeatherType_Snow     , sword::WeatherAttributes::snow                      );
PHY_Precipitation PHY_Precipitation::smoke_    ( "Fumigene"          , eWeatherType_Smoke    , (sword::WeatherAttributes::EnumPrecipitationType)-1 ); // $$$$ MCO : huhh ?!

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Precipitation::Initialize()
{
//    MT_LOG_INFO_MSG( "Initializing precipitations" );
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

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Precipitation::PHY_Precipitation( const std::string& strName, E_WeatherType nType, sword::WeatherAttributes::EnumPrecipitationType nAsnID )
    : strName_( strName )
    , nType_  ( nType   )
    , nAsnID_ ( nAsnID  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Precipitation::~PHY_Precipitation()
{
    // NOTHING
}
