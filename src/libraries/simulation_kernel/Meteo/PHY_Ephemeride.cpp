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
#include "MIL_AgentServer.h"
#include "xeumeuleu/xml.h"
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;


//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::UpdateNight
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool PHY_Ephemeride::UpdateNight( unsigned int date )
{
    bpt::ptime         pdate( bpt::from_time_t( date ) );
    bpt::time_duration time = pdate.time_of_day();
    std::pair<int,int> currentTime( time.hours(), time.minutes() );

    bool wasNight = bIsNight_;
    bIsNight_ = currentTime < sunriseTime_ && !( currentTime < sunsetTime_ );
    return bIsNight_ != wasNight;
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride constructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::PHY_Ephemeride( xml::xistream& xis )
    : pNightBase_( 0 )
    , bIsNight_  ( false )
{
    std::string sunRise, sunSet, moon, date;
    xis >> xml::start( "ephemerides" )
            >> xml::attribute( "sunrise", sunRise )
            >> xml::attribute( "sunset", sunSet )
            >> xml::attribute( "moon", moon )
        >> xml::end()
        >> xml::start( "exercise-date" )
            >> xml::attribute( "value", date )
        >> xml::end();
    unsigned int time = ( bpt::from_iso_string( date ) - bpt::from_time_t( 0 ) ).total_seconds();
    MIL_AgentServer::GetWorkspace().SetInitialRealTime( time );
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
    UpdateNight( MIL_AgentServer::GetWorkspace().GetRealTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride destructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::~PHY_Ephemeride()
{

}



