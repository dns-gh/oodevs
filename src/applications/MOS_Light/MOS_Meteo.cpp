//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo.cpp $
// $Author: Age $
// $Modtime: 7/02/05 11:39 $
// $Revision: 3 $
// $Workfile: MOS_Meteo.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_Meteo.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo.inl"
#endif


#include "MOS_App.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MOS_ASN_Messages.h"


MT_ObjectRecycler< MOS_Meteo > MOS_Meteo::recycler_;

MOS_Meteo::T_MeteoVector MOS_Meteo::vInstanciatedMeteo_;

//-----------------------------------------------------------------------------
// Name: MOS_Meteo::UpdateNight::sEphemeride
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool MOS_Meteo::sEphemeride::UpdateNight()
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
// Name: MOS_Meteo::sEphemeride::Initialize
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo::sEphemeride::Initialize( int sunriseHour, int sunriseMinute, int sunsetHour, int sunsetMinute, E_LightingType nNightBase )
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
// Name: MOS_Meteo::Create
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
MOS_Meteo* MOS_Meteo::Create( MT_InputArchive_ABC& archive )
{
    MOS_Meteo *pElt = recycler_.GetElement();
    if ( !pElt )
        pElt = new MOS_Meteo();

    vInstanciatedMeteo_.push_back( pElt );
    pElt->Initialize( archive );
    return pElt;
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo::Create
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
MOS_Meteo* MOS_Meteo::Create( const ASN1T_MeteoAttributs& asnMsg )
{
    MOS_Meteo *pElt = recycler_.GetElement();
    if ( !pElt )
        pElt = new MOS_Meteo();
    
    vInstanciatedMeteo_.push_back( pElt );
    if ( pElt->Initialize( asnMsg ) )
        return pElt;
    else if ( !recycler_.AddElement( *pElt ) )
        delete pElt;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::Destroy
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
bool MOS_Meteo::Destroy()
{
    Terminate();
    vInstanciatedMeteo_.erase( std::find( vInstanciatedMeteo_.begin(), vInstanciatedMeteo_.end(), this ) );
    if ( !recycler_.AddElement( *this ) )
        delete this;
    return true;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
MOS_Meteo::MOS_Meteo()
    : nEclairement_   ( eLightingJourSansNuage )
    , nPrecipitation_ ( eWeatherTypeNone )
    , nRefCount_      ( 0 )
{
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo destructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
MOS_Meteo::~MOS_Meteo()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::Initialize
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void MOS_Meteo::Initialize( MT_InputArchive_ABC& archive )
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
    if ( !archive.ReadField( "PlancherCouvertureNuageuse", nPlancherCouvertureNuageuse_ ) )
        throw MT_ScipioException( "MOS_Meteo::Initialize", __FILE__, __LINE__, "Can't read section 'Meteo::PlancherCouvertureNuageuse'", archive.RetrieveLastError()->GetInfo() );

    // Plafond de couverture nuageuse
    if ( !archive.ReadField( "PlafondCouvertureNuageuse", nPlafondCouvertureNuageuse_ ) )
        throw MT_ScipioException( "MOS_Meteo::Initialize", __FILE__, __LINE__, "Can't read section 'Meteo::PlafondCouvertureNuageuse'", archive.RetrieveLastError()->GetInfo() );

    // Densite moyenne de couverture nuageuse
    uint nVal;
    if ( !archive.ReadField( "DensiteMoyenneCouvertureNuageuse", nVal ) )
        throw MT_ScipioException( "MOS_Meteo::Initialize", __FILE__, __LINE__, "Can't read section 'Meteo::DensiteMoyenneCouvertureNuageuse'", archive.RetrieveLastError()->GetInfo() );
    rDensiteCouvertureNuageuse_ = std::min( nVal, (uint)100 ) / 100.;

    // Précipitation
    std::string strVal;
    if ( !archive.ReadField( "Precipitation", strVal ) )
        throw MT_ScipioException( "MOS_Meteo::Initialize", __FILE__, __LINE__, "Can't read section 'Meteo::Precipitation'", archive.RetrieveLastError()->GetInfo() );
    
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
        throw MT_ScipioException( "MOS_Meteo::Initialize", __FILE__, __LINE__, MT_FormatString( "Unknown Precipitation type '%s'", strVal.c_str() ) );

    // Eclairement
    ComputeTimeCategory( MOS_App::GetApp().GetRawVisionData().GetEphemeride() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::Initialize
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
bool MOS_Meteo::Initialize( const ASN1T_MeteoAttributs& asnMsg )
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
    ComputeTimeCategory( MOS_App::GetApp().GetRawVisionData().GetEphemeride() );

    return true;
}

//-----------------------------------------------------------------------------
// Name: MOS_Meteo::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
E_LightingType MOS_Meteo::sEphemeride::GetCurrentTimeBase() const
{
    return bIsNight_ ? nNightBase_ : eLightingJourSansNuage;
}

//-----------------------------------------------------------------------------
// Name: MOS_Meteo::ComputeTimeCategory
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo::ComputeTimeCategory( const sEphemeride& ephemeride )
{
    nEclairement_ = (E_LightingType)std::min( (uint)( eNbrLightingType - 1 ), (uint)( ephemeride.GetCurrentTimeBase() + std::min( (uint)( rDensiteCouvertureNuageuse_ * ( nPlafondCouvertureNuageuse_ - nPlancherCouvertureNuageuse_ ) / 2000 ), (uint)4 ) ) );
}
