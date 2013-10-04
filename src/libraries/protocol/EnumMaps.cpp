// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "EnumMaps.h"
#include <sstream>
#include <utility>

using namespace sword;

namespace protocol
{

namespace
{

#define COUNT_OF( array ) sizeof( array )/sizeof( *array )

template< typename E1, typename E2 >
E1 FindE1FromE2( E2 e2, const std::pair< E1, E2 >* pairs, size_t pairsCount, const char* name )
{
    for( size_t i = 0; i != pairsCount; ++i )
        if( pairs[ i ].second == e2 )
            return pairs[ i ].first;
    std::stringstream err;
    err << "cannot map " << name << " value: " << e2;
    throw std::logic_error( err.str() );
}

template< typename E1, typename E2 >
E2 FindE2FromE1( E1 e1, const std::pair< E1, E2 >* pairs, size_t pairsCount, const char* name )
{
    for( size_t i = 0; i != pairsCount; ++i )
        if( pairs[ i ].first == e1 )
            return pairs[ i ].second;
    std::stringstream err;
    err << "cannot map " << name << " value: " << e1;
    throw std::logic_error( err.str() );
}

typedef std::pair< E_LightingType, WeatherAttributes::EnumLightingType > LightingPair;

const LightingPair lightings[] =
{
    LightingPair( eLightingType_JourSansNuage          , WeatherAttributes::clear_day ),
    LightingPair( eLightingType_JourPeuNuageux         , WeatherAttributes::slightly_cloudy_day ),
    LightingPair( eLightingType_JourMoyennementNuageux , WeatherAttributes::mildly_cloudy_day ),
    LightingPair( eLightingType_JourAssezNuageux       , WeatherAttributes::fairly_cloudy_day ),
    LightingPair( eLightingType_JourTresNuageux        , WeatherAttributes::very_cloudy_day ),
    LightingPair( eLightingType_NuitPleineLune         , WeatherAttributes::full_moon_night ),
    LightingPair( eLightingType_NuitTroisQuartDeLune   , WeatherAttributes::three_quater_moon_night ),
    LightingPair( eLightingType_NuitDemiLune           , WeatherAttributes::half_quater_moon_night ),
    LightingPair( eLightingType_NuitQuartDeLune        , WeatherAttributes::quater_moon_night ),
    LightingPair( eLightingType_NuitNouvelleLune       , WeatherAttributes::new_moon_night ),
    LightingPair( eLightingType_Eclairant              , WeatherAttributes::artificial_light ),
};
const size_t lightingsSize = COUNT_OF( lightings );

} // namespce

E_LightingType FromProtoLighting( WeatherAttributes::EnumLightingType value )
{
    return FindE1FromE2( value, lightings, lightingsSize, "protobuf lighting" );
}

WeatherAttributes::EnumLightingType ToProtoLighting( E_LightingType value )
{
    return FindE2FromE1( value, lightings, lightingsSize, "ent lighting" );
}

}  // protocol

