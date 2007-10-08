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

#include "simulation_kernel_pch.h"
#include "PHY_Ephemeride.h"
#include "xeumeuleu/xml.h"

using namespace xml;

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
PHY_Ephemeride::PHY_Ephemeride( xml::xistream& xis )
    : pNightBase_( 0 )
    , bIsNight_  ( false )
{
    std::string sunRise, sunSet, moon;
    xis >> start( "ephemerides" )
            >> attribute( "sunrise", sunRise )
            >> attribute( "sunset", sunSet )
            >> attribute( "moon", moon )
        >> xml::end();

    {
        char tmp = 0;
        std::istringstream strTmp( sunRise );
        strTmp >> sunriseTime_.first >> tmp >> sunriseTime_.second;
        if ( tmp != 'h' || sunriseTime_.first < 0 || sunriseTime_.first > 23 || sunriseTime_.second < 0 || sunriseTime_.second > 59 )
            xis.error( "Bad time format (use 00h00)" );
    }

    {
        char tmp = 0;
        std::istringstream strTmp( sunSet );
        strTmp >> sunsetTime_.first >> tmp >> sunsetTime_.second;
        if ( tmp != 'h' || sunsetTime_.first < 0 || sunsetTime_.first > 23 || sunsetTime_.second < 0 || sunsetTime_.second > 59 )
            xis.error( "Bad time format (use 00h00)" );
    }
    
    pNightBase_ = PHY_Lighting::FindLighting( moon );
    if( !pNightBase_ )
        xis.error( "Unknown lighting '" + moon + "'" );

    if( sunriseTime_ >= sunsetTime_  )
        xis.error( "Sunrise time should be before sunset time" );
    UpdateNight();
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride destructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::~PHY_Ephemeride()
{

}



