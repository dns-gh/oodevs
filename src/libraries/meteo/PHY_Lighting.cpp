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

const PHY_Lighting PHY_Lighting::jourSansNuage_         ( "JourSansNuage"         , eLightingType_JourSansNuage          , &jourPeuNuageux_ );
const PHY_Lighting PHY_Lighting::jourPeuNuageux_        ( "JourPeuNuageux"        , eLightingType_JourPeuNuageux         , &jourMoyennementNuageux_ );
const PHY_Lighting PHY_Lighting::jourMoyennementNuageux_( "JourMoyennementNuageux", eLightingType_JourMoyennementNuageux , &jourAssezNuageux_ );
const PHY_Lighting PHY_Lighting::jourAssezNuageux_      ( "JourAssezNuageux"      , eLightingType_JourAssezNuageux       , &jourTresNuageux_ );
const PHY_Lighting PHY_Lighting::jourTresNuageux_       ( "JourTresNuageux"       , eLightingType_JourTresNuageux        , 0 );
const PHY_Lighting PHY_Lighting::nuitPleineLune_        ( "NuitPleineLune"        , eLightingType_NuitPleineLune         , &nuitTroisQuartDeLune_ );
const PHY_Lighting PHY_Lighting::nuitTroisQuartDeLune_  ( "NuitTroisQuartDeLune"  , eLightingType_NuitTroisQuartDeLune   , &nuitDemiLune_ );
const PHY_Lighting PHY_Lighting::nuitDemiLune_          ( "NuitDemiLune"          , eLightingType_NuitDemiLune           , &nuitQuartDeLune_ );
const PHY_Lighting PHY_Lighting::nuitQuartDeLune_       ( "NuitQuartDeLune"       , eLightingType_NuitQuartDeLune        , &nuitNouvelleLune_ );
const PHY_Lighting PHY_Lighting::nuitNouvelleLune_      ( "NuitNouvelleLune"      , eLightingType_NuitNouvelleLune       , 0 );
const PHY_Lighting PHY_Lighting::eclairant_             ( "Eclairant"             , eLightingType_Eclairant              , 0 );

namespace
{

std::map< std::string, const PHY_Lighting* > MakeLightings()
{
    std::map< std::string, const PHY_Lighting* > lightings;
    lightings[ PHY_Lighting::jourSansNuage_         .GetName() ] = &PHY_Lighting::jourSansNuage_;
    lightings[ PHY_Lighting::jourPeuNuageux_        .GetName() ] = &PHY_Lighting::jourPeuNuageux_;
    lightings[ PHY_Lighting::jourMoyennementNuageux_.GetName() ] = &PHY_Lighting::jourMoyennementNuageux_;
    lightings[ PHY_Lighting::jourAssezNuageux_      .GetName() ] = &PHY_Lighting::jourAssezNuageux_;
    lightings[ PHY_Lighting::jourTresNuageux_       .GetName() ] = &PHY_Lighting::jourTresNuageux_;
    lightings[ PHY_Lighting::nuitPleineLune_        .GetName() ] = &PHY_Lighting::nuitPleineLune_;
    lightings[ PHY_Lighting::nuitTroisQuartDeLune_  .GetName() ] = &PHY_Lighting::nuitTroisQuartDeLune_;
    lightings[ PHY_Lighting::nuitDemiLune_          .GetName() ] = &PHY_Lighting::nuitDemiLune_;
    lightings[ PHY_Lighting::nuitQuartDeLune_       .GetName() ] = &PHY_Lighting::nuitQuartDeLune_;
    lightings[ PHY_Lighting::nuitNouvelleLune_      .GetName() ] = &PHY_Lighting::nuitNouvelleLune_;
    lightings[ PHY_Lighting::eclairant_             .GetName() ] = &PHY_Lighting::eclairant_;
    return lightings;
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: PHY_Lighting constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Lighting::PHY_Lighting( const std::string& strName, E_LightingType nType, const PHY_Lighting* pNextDegradedLighting )
    : strName_              ( strName )
    , nType_                ( nType )
    , pNextDegradedLighting_( pNextDegradedLighting )
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

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetLightings
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::map< std::string, const PHY_Lighting* >& PHY_Lighting::GetLightings()
{
    static const std::map< std::string, const PHY_Lighting* > lightings = MakeLightings();
    return lightings;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindLighting
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Lighting* PHY_Lighting::FindLighting( const std::string& strName )
{
    const auto& lightings = PHY_Lighting::GetLightings();
    const auto it = lightings.find( strName );
    if( it == lightings.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindPrecipitation
// Created: SLG 2010-05-26
// -----------------------------------------------------------------------------
const PHY_Lighting* PHY_Lighting::FindLighting( E_LightingType value )
{
    const auto& lightings = PHY_Lighting::GetLightings();
    for( auto it = lightings.begin(); it != lightings.end(); ++it )
    {
        const PHY_Lighting& lighting = *it->second;
        if( lighting.GetID() == value )
            return &lighting;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Lighting::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
E_LightingType PHY_Lighting::GetID() const
{
    return nType_;
}
