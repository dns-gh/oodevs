// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Lighting.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_Lighting.cpp $
//
// *****************************************************************************

#include "PHY_Lighting.h"

using namespace weather;

PHY_Lighting::T_LightingMap PHY_Lighting::lightings_;

PHY_Lighting PHY_Lighting::jourSansNuage_         ( "JourSansNuage"         , eLightingType_JourSansNuage          , &jourPeuNuageux_         , sword::WeatherAttributes::clear_day );
PHY_Lighting PHY_Lighting::jourPeuNuageux_        ( "JourPeuNuageux"        , eLightingType_JourPeuNuageux         , &jourMoyennementNuageux_ , sword::WeatherAttributes::slightly_cloudy_day );
PHY_Lighting PHY_Lighting::jourMoyennementNuageux_( "JourMoyennementNuageux", eLightingType_JourMoyennementNuageux , &jourAssezNuageux_       , sword::WeatherAttributes::mildly_cloudy_day );
PHY_Lighting PHY_Lighting::jourAssezNuageux_      ( "JourAssezNuageux"      , eLightingType_JourAssezNuageux       , &jourTresNuageux_        , sword::WeatherAttributes::fairly_cloudy_day );
PHY_Lighting PHY_Lighting::jourTresNuageux_       ( "JourTresNuageux"       , eLightingType_JourTresNuageux        , 0                        , sword::WeatherAttributes::very_cloudy_day );
PHY_Lighting PHY_Lighting::nuitPleineLune_        ( "NuitPleineLune"        , eLightingType_NuitPleineLune         , &nuitTroisQuartDeLune_   , sword::WeatherAttributes::full_moon_night );
PHY_Lighting PHY_Lighting::nuitTroisQuartDeLune_  ( "NuitTroisQuartDeLune"  , eLightingType_NuitTroisQuartDeLune   , &nuitDemiLune_           , sword::WeatherAttributes::three_quater_moon_night );
PHY_Lighting PHY_Lighting::nuitDemiLune_          ( "NuitDemiLune"          , eLightingType_NuitDemiLune           , &nuitQuartDeLune_        , sword::WeatherAttributes::half_quater_moon_night );
PHY_Lighting PHY_Lighting::nuitQuartDeLune_       ( "NuitQuartDeLune"       , eLightingType_NuitQuartDeLune        , &nuitNouvelleLune_       , sword::WeatherAttributes::quater_moon_night );
PHY_Lighting PHY_Lighting::nuitNouvelleLune_      ( "NuitNouvelleLune"      , eLightingType_NuitNouvelleLune       , 0                        , sword::WeatherAttributes::new_moon_night );
PHY_Lighting PHY_Lighting::eclairant_             ( "Eclairant"             , eLightingType_Eclairant              , 0                        , sword::WeatherAttributes::artificial_light );

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Lighting::Initialize()
{
    // MT_LOG_INFO_MSG( "Initializing lightings" );
    lightings_[ jourSansNuage_         .GetName() ] = &jourSansNuage_;
    lightings_[ jourPeuNuageux_        .GetName() ] = &jourPeuNuageux_;
    lightings_[ jourMoyennementNuageux_.GetName() ] = &jourMoyennementNuageux_;
    lightings_[ jourAssezNuageux_      .GetName() ] = &jourAssezNuageux_;
    lightings_[ jourTresNuageux_       .GetName() ] = &jourTresNuageux_;
    lightings_[ nuitPleineLune_        .GetName() ] = &nuitPleineLune_;
    lightings_[ nuitTroisQuartDeLune_  .GetName() ] = &nuitTroisQuartDeLune_;
    lightings_[ nuitDemiLune_          .GetName() ] = &nuitDemiLune_;
    lightings_[ nuitQuartDeLune_       .GetName() ] = &nuitQuartDeLune_;
    lightings_[ nuitNouvelleLune_      .GetName() ] = &nuitNouvelleLune_;
    lightings_[ eclairant_             .GetName() ] = &eclairant_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Lighting::Terminate()
{
    lightings_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Lighting::PHY_Lighting( const std::string& strName, E_LightingType nType, const PHY_Lighting* pNextDegradedLighting, sword::WeatherAttributes::EnumLightingType nAsnID )
    : strName_              ( strName )
    , nType_                ( nType )
    , pNextDegradedLighting_( pNextDegradedLighting )
    , nAsnID_               ( nAsnID )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Lighting::~PHY_Lighting()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetDegradedLighting
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const PHY_Lighting& PHY_Lighting::GetDegradedLighting( unsigned int nDegradationFactor ) const
{
    if( nDegradationFactor == 0 || !pNextDegradedLighting_ )
        return *this;
    return pNextDegradedLighting_->GetDegradedLighting( nDegradationFactor - 1 );
}
