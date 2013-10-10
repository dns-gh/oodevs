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

PHY_Lighting PHY_Lighting::jourSansNuage_         ( "JourSansNuage"         , eLightingType_JourSansNuage          , &jourPeuNuageux_ );
PHY_Lighting PHY_Lighting::jourPeuNuageux_        ( "JourPeuNuageux"        , eLightingType_JourPeuNuageux         , &jourMoyennementNuageux_ );
PHY_Lighting PHY_Lighting::jourMoyennementNuageux_( "JourMoyennementNuageux", eLightingType_JourMoyennementNuageux , &jourAssezNuageux_ );
PHY_Lighting PHY_Lighting::jourAssezNuageux_      ( "JourAssezNuageux"      , eLightingType_JourAssezNuageux       , &jourTresNuageux_ );
PHY_Lighting PHY_Lighting::jourTresNuageux_       ( "JourTresNuageux"       , eLightingType_JourTresNuageux        , 0 );
PHY_Lighting PHY_Lighting::nuitPleineLune_        ( "NuitPleineLune"        , eLightingType_NuitPleineLune         , &nuitTroisQuartDeLune_ );
PHY_Lighting PHY_Lighting::nuitTroisQuartDeLune_  ( "NuitTroisQuartDeLune"  , eLightingType_NuitTroisQuartDeLune   , &nuitDemiLune_ );
PHY_Lighting PHY_Lighting::nuitDemiLune_          ( "NuitDemiLune"          , eLightingType_NuitDemiLune           , &nuitQuartDeLune_ );
PHY_Lighting PHY_Lighting::nuitQuartDeLune_       ( "NuitQuartDeLune"       , eLightingType_NuitQuartDeLune        , &nuitNouvelleLune_ );
PHY_Lighting PHY_Lighting::nuitNouvelleLune_      ( "NuitNouvelleLune"      , eLightingType_NuitNouvelleLune       , 0 );
PHY_Lighting PHY_Lighting::eclairant_             ( "Eclairant"             , eLightingType_Eclairant              , 0 );

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
const PHY_Lighting::T_LightingMap& PHY_Lighting::GetLightings()
{
    return lightings_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindLighting
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Lighting* PHY_Lighting::FindLighting( const std::string& strName )
{
    CIT_LightingMap it = lightings_.find( strName );
    if( it == lightings_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindPrecipitation
// Created: SLG 2010-05-26
// -----------------------------------------------------------------------------
const PHY_Lighting* PHY_Lighting::FindLighting( E_LightingType value )
{
    for( auto it = lightings_.begin(); it != lightings_.end(); ++it )
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
