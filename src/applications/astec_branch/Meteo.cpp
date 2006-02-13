//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo.cpp $
// $Author: Age $
// $Modtime: 14/04/05 10:29 $
// $Revision: 4 $
// $Workfile: Meteo.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Meteo.h"


#include "App.h"

#include "MT_Tools/MT_ScipioException.h"
#include "ASN_Messages.h"


MT_ObjectRecycler< Meteo > Meteo::recycler_;

Meteo::T_MeteoVector Meteo::vInstanciatedMeteo_;

//-----------------------------------------------------------------------------
// Name: Meteo::UpdateNight::sEphemeride
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool Meteo::sEphemeride::UpdateNight()
{
    time_t t;
    time( &t );
    struct tm * time;
    time = localtime( &t );
    std::pair<int,int> currentTime( time->tm_hour, time->tm_min );

    bool bIsNight = currentTime < sunriseTime_ ? true  :
                    currentTime < sunsetTime_  ? false : true;

    if ( bIsNight != bIsNight_ )
    {
        bIsNight_ = bIsNight;
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: Meteo::sEphemeride::Initialize
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void Meteo::sEphemeride::Initialize( int sunriseHour, int sunriseMinute, int sunsetHour, int sunsetMinute, E_LightingType nNightBase )
{
    sunriseTime_.first  = sunriseHour;
    sunriseTime_.second = sunriseMinute;    
    sunsetTime_.first   = sunsetHour;    
    sunsetTime_.second  = sunsetMinute;    
    nNightBase_    = nNightBase;    
    assert( sunriseTime_ < sunsetTime_ );
    UpdateNight();
}


//-----------------------------------------------------------------------------
// Name: Meteo::Create
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
Meteo* Meteo::Create( InputArchive& archive )
{
    Meteo *pElt = recycler_.GetElement();
    if ( !pElt )
        pElt = new Meteo();

    vInstanciatedMeteo_.push_back( pElt );
    pElt->Initialize( archive );
    return pElt;
}



//-----------------------------------------------------------------------------
// Name: Meteo::Create
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
Meteo* Meteo::Create( const ASN1T_MeteoAttributs& asnMsg )
{
    Meteo *pElt = recycler_.GetElement();
    if ( !pElt )
        pElt = new Meteo();
    
    vInstanciatedMeteo_.push_back( pElt );
    if ( pElt->Initialize( asnMsg ) )
        return pElt;
    else if ( !recycler_.AddElement( *pElt ) )
        delete pElt;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: Meteo::Destroy
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
bool Meteo::Destroy()
{
    Terminate();
    vInstanciatedMeteo_.erase( std::find( vInstanciatedMeteo_.begin(), vInstanciatedMeteo_.end(), this ) );
    if ( !recycler_.AddElement( *this ) )
        delete this;
    return true;
}


//-----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
Meteo::Meteo()
    : nEclairement_   ( eLightingJourSansNuage )
    , nPrecipitation_ ( eWeatherTypeNone )
    , nRefCount_      ( 0 )
{
}



//-----------------------------------------------------------------------------
// Name: Meteo destructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
Meteo::~Meteo()
{
}


//-----------------------------------------------------------------------------
// Name: Meteo::Initialize
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void Meteo::Initialize( InputArchive& archive )
{
    assert( !nRefCount_  );

    // Temperature
    // Non utilisé

    // Gardian de temperature
    // Non utilisé

    // Vitesse du vent
    // Non utilisé

    // Direction du vent
    // non utilisé

    // Plancher de couverture nuageuse
    archive.ReadField( "PlancherCouvertureNuageuse", nPlancherCouvertureNuageuse_ );
    // Plafond de couverture nuageuse
    archive.ReadField( "PlafondCouvertureNuageuse", nPlafondCouvertureNuageuse_ );
    // Densite moyenne de couverture nuageuse
    uint nVal;
    archive.ReadField( "DensiteMoyenneCouvertureNuageuse", nVal );
    rDensiteCouvertureNuageuse_ = std::min( nVal, (uint)100 ) / 100.;

    // Précipitation
    std::string strVal;
    archive.ReadField( "Precipitation", strVal );
    if ( strVal == "PasDePrecipitation" )
        nPrecipitation_ = eWeatherTypeNone            ;
    else if ( strVal == "TempeteDeNeigeOuPluie" )
        nPrecipitation_ = eWeatherTypeRainOrSnowStorm ;
    else if ( strVal == "Brouillard" )
        nPrecipitation_ = eWeatherTypeFog             ;
    else if ( strVal == "Crachin" )
        nPrecipitation_ = eWeatherTypeDrizzle         ;
    else if ( strVal == "Pluie" )
        nPrecipitation_ = eWeatherTypeRain            ;
    else if ( strVal == "Neige" )
        nPrecipitation_ = eWeatherTypeSnow            ;
    else
        throw MT_ScipioException( "Meteo::Initialize", __FILE__, __LINE__, MT_FormatString( "Unknown Precipitation type '%s'", strVal.c_str() ) );

    // Eclairement
    ComputeTimeCategory( App::GetApp().GetRawVisionData().GetEphemeride() );
}


//-----------------------------------------------------------------------------
// Name: Meteo::Initialize
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
bool Meteo::Initialize( const ASN1T_MeteoAttributs& asnMsg )
{
    assert( !nRefCount_ );

    // Temperature
    // Non utilisé

    // Gardian de temperature
    // Non utilisé

    // Vitesse du vent
    // Non utilisé

    // Direction du vent
    // non utilisé

    // Plancher de couverture nuageuse
    nPlancherCouvertureNuageuse_ = asnMsg.plancher_couverture_nuageuse;

    // Plafond de couverture nuageuse
    nPlafondCouvertureNuageuse_ = asnMsg.plafond_couverture_nuageuse;

    // Densite moyenne de couverture nuageuse
    rDensiteCouvertureNuageuse_ = std::min( std::max( asnMsg.densite_moyenne_couverture_nuageuse, 0 ), 100 ) / 100.;

    // Précipitation
    assert( asnMsg.precipitation >= 0 && asnMsg.precipitation < eNbrWeatherType );
    nPrecipitation_ = (E_WeatherType)asnMsg.precipitation;

    // Eclairement
    ComputeTimeCategory( App::GetApp().GetRawVisionData().GetEphemeride() );

    return true;
}

//-----------------------------------------------------------------------------
// Name: Meteo::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
E_LightingType Meteo::sEphemeride::GetCurrentTimeBase() const
{
    return bIsNight_ ? nNightBase_ : eLightingJourSansNuage;
}

//-----------------------------------------------------------------------------
// Name: Meteo::ComputeTimeCategory
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void Meteo::ComputeTimeCategory( const sEphemeride& ephemeride )
{
    nEclairement_ = (E_LightingType)std::min( (uint)( eNbrLightingType - 1 ), (uint)( ephemeride.GetCurrentTimeBase() + std::min( (uint)( rDensiteCouvertureNuageuse_ * ( nPlafondCouvertureNuageuse_ - nPlancherCouvertureNuageuse_ ) / 2000 ), (uint)4 ) ) );
}
