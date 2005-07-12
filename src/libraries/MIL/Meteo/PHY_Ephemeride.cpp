//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/PHY_Ephemeride.cpp $
// $Author: Nld $
// $Modtime: 8/03/05 11:43 $
// $Revision: 3 $
// $Workfile: PHY_Ephemeride.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"
#include "PHY_Ephemeride.h"

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::UpdateNight
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool PHY_Ephemeride::UpdateNight()
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

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride constructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::PHY_Ephemeride( MIL_InputArchive& archive )
    : pNightBase_( 0 )
    , bIsNight_  ( false )
{
    archive.Section( "Ephemeride" );

    std::string strVal;

    // HeureLeverSoleil
    archive.ReadField( "HeureLeverSoleil", strVal );
    char tmp = 0;
    {
        std::istringstream strTmp( strVal );
        strTmp >> sunriseTime_.first >> tmp >> sunriseTime_.second;
    }
    if ( tmp != 'h' || sunriseTime_.first < 0 || sunriseTime_.first > 23 || sunriseTime_.second < 0 || sunriseTime_.second > 59 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Bad time format (use 00h00)", archive.GetContext() );

    // HeureCoucherSoleil
    archive.ReadField( "HeureCoucherSoleil", strVal );
    tmp = 0;
    {
        std::istringstream strTmp( strVal );
        strTmp >> sunsetTime_.first >> tmp >> sunsetTime_.second;
    }
    if ( tmp != 'h' || sunsetTime_.first < 0 || sunsetTime_.first > 23 || sunsetTime_.second < 0 || sunsetTime_.second > 59 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Bad time format (use 00h00)", archive.GetContext() );

    // Lune
    archive.ReadField( "Lune", strVal );
    pNightBase_ = PHY_Lighting::FindLighting( strVal );
    if( !pNightBase_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown lighting '%s'", strVal.c_str() ), archive.GetContext() );

    archive.EndSection(); // Ephemeride

    if( sunriseTime_ >= sunsetTime_  )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sunrise time should be before sunset time", archive.GetContext() );
    UpdateNight();
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride destructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::~PHY_Ephemeride()
{

}



