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

const PHY_Precipitation PHY_Precipitation::none_     ( "PasDePrecipitation", eWeatherType_None );
const PHY_Precipitation PHY_Precipitation::sandStorm_( "TempeteDeSable"    , eWeatherType_SandStorm );
const PHY_Precipitation PHY_Precipitation::fog_      ( "Brouillard"        , eWeatherType_Fog );
const PHY_Precipitation PHY_Precipitation::drizzle_  ( "Crachin"           , eWeatherType_Drizzle );
const PHY_Precipitation PHY_Precipitation::rain_     ( "Pluie"             , eWeatherType_Rain );
const PHY_Precipitation PHY_Precipitation::snow_     ( "Neige"             , eWeatherType_Snow );
const PHY_Precipitation PHY_Precipitation::smoke_    ( "Fumigene"          , eWeatherType_Smoke );

namespace
{

std::map< std::string, const PHY_Precipitation* > MakePrecipitations()
{
    std::map< std::string, const PHY_Precipitation* > precipitations;
    precipitations[ PHY_Precipitation::none_     .GetName() ] = &PHY_Precipitation::none_;
    precipitations[ PHY_Precipitation::sandStorm_.GetName() ] = &PHY_Precipitation::sandStorm_;
    precipitations[ PHY_Precipitation::fog_      .GetName() ] = &PHY_Precipitation::fog_;
    precipitations[ PHY_Precipitation::drizzle_  .GetName() ] = &PHY_Precipitation::drizzle_;
    precipitations[ PHY_Precipitation::rain_     .GetName() ] = &PHY_Precipitation::rain_;
    precipitations[ PHY_Precipitation::snow_     .GetName() ] = &PHY_Precipitation::snow_;
    precipitations[ PHY_Precipitation::smoke_    .GetName() ] = &PHY_Precipitation::smoke_;
    return precipitations;
}

}  // namespace

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
const std::map< std::string, const PHY_Precipitation* >& PHY_Precipitation::GetPrecipitations()
{
    static const std::map< std::string, const PHY_Precipitation* > p = MakePrecipitations();
    return p;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( const std::string& strName )
{
    const auto& precipitations = PHY_Precipitation::GetPrecipitations();
    const auto it = precipitations.find( strName );
    if( it == precipitations.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( E_WeatherType type )
{
    const auto& precipitations = PHY_Precipitation::GetPrecipitations();
    for( auto it = precipitations.begin(); it != precipitations.end(); ++it )
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
