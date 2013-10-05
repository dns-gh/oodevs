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
PHY_Precipitation PHY_Precipitation::none_     ( "PasDePrecipitation", eWeatherType_None );
PHY_Precipitation PHY_Precipitation::sandStorm_( "TempeteDeSable"    , eWeatherType_SandStorm );
PHY_Precipitation PHY_Precipitation::fog_      ( "Brouillard"        , eWeatherType_Fog );
PHY_Precipitation PHY_Precipitation::drizzle_  ( "Crachin"           , eWeatherType_Drizzle );
PHY_Precipitation PHY_Precipitation::rain_     ( "Pluie"             , eWeatherType_Rain );
PHY_Precipitation PHY_Precipitation::snow_     ( "Neige"             , eWeatherType_Snow );
PHY_Precipitation PHY_Precipitation::smoke_    ( "Fumigene"          , eWeatherType_Smoke );

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
PHY_Precipitation::PHY_Precipitation( const std::string& strName, E_WeatherType nType )
    : strName_( strName )
    , nType_  ( nType   )
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

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetPrecipitation
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Precipitation::T_PrecipitationMap& PHY_Precipitation::GetPrecipitations()
{
    return precipitations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( const std::string& strName )
{
    CIT_PrecipitationMap it = precipitations_.find( strName );
    if( it == precipitations_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( E_WeatherType type )
{
    for( auto it = precipitations_.begin(); it != precipitations_.end(); ++it )
    {
        const PHY_Precipitation& precipitation = *it->second;
        if( precipitation.GetID() == type )
            return &precipitation;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Precipitation::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
E_WeatherType PHY_Precipitation::GetID() const
{
    return nType_;
}
