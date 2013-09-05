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
#include "meteo/PHY_Lighting.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
PHY_Ephemeride::PHY_Ephemeride()
    : pDayBase_( 0 )
    , pNightBase_( 0 )
    , bIsNight_( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride constructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::PHY_Ephemeride( xml::xistream& xis )
    : bIsNight_( false )
{
    xis >> xml::start( "ephemerides" );
    std::string sunRise = xis.attribute< std::string >( "sunrise" );
    std::string sunSet = xis.attribute< std::string >( "sunset" );
    if( xis.has_attribute( "moon" ) )
    {
        pDayBase_ = &weather::PHY_Lighting::jourSansNuage_;
        pNightBase_ = weather::PHY_Lighting::FindLighting( xis.attribute< std::string >( "moon" ) );
    }
    else
    {
        pDayBase_ = weather::PHY_Lighting::FindLighting( xis.attribute< std::string >( "day-lighting" ) );
        pNightBase_ = weather::PHY_Lighting::FindLighting( xis.attribute< std::string >( "night-lighting" ) );
    }
    if( !pDayBase_ || !pNightBase_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown lighting" );
    std::string date;
    xis >> xml::end
        >> xml::start( "exercise-date" )
            >> xml::attribute( "value", date )
        >> xml::end;
    const unsigned int time = ( bpt::from_iso_string( date ) - bpt::from_time_t( 0 ) ).total_seconds();
    MIL_AgentServer::GetWorkspace().SetInitialRealTime( time );
    {
        char tmp = 0;
        std::istringstream strTmp( sunRise );
        strTmp >> sunriseTime_.first >> tmp >> sunriseTime_.second;
        if( tmp != 'h' || sunriseTime_.first < 0 || sunriseTime_.first > 23 || sunriseTime_.second < 0 || sunriseTime_.second > 59 )
            throw MASA_EXCEPTION( xis.context() + "Bad time format (use 00h00)" );
    }
    {
        char tmp = 0;
        std::istringstream strTmp( sunSet );
        strTmp >> sunsetTime_.first >> tmp >> sunsetTime_.second;
        if( tmp != 'h' || sunsetTime_.first < 0 || sunsetTime_.first > 23 || sunsetTime_.second < 0 || sunsetTime_.second > 59 )
            throw MASA_EXCEPTION( xis.context() + "Bad time format (use 00h00)" );
    }
    if( sunriseTime_ >= sunsetTime_  )
        throw MASA_EXCEPTION( xis.context() + "Sunrise time should be before sunset time" );
    UpdateNight( MIL_Time_ABC::GetTime().GetRealTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride destructor
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
PHY_Ephemeride::~PHY_Ephemeride()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride::load
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_Ephemeride::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int dayId;
    unsigned int nightId;
    file >> bIsNight_
         >> sunriseTime_
         >> sunsetTime_
         >> dayId
         >> nightId;
    pDayBase_ = weather::PHY_Lighting::FindLighting( static_cast< sword::WeatherAttributes::EnumLightingType >( dayId ) );
    pNightBase_ = weather::PHY_Lighting::FindLighting( static_cast< sword::WeatherAttributes::EnumLightingType >( nightId ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride::save
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_Ephemeride::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned int dayId = pDayBase_->GetAsnID();
    unsigned int nightId = pNightBase_->GetAsnID();
    file << bIsNight_
         << sunriseTime_
         << sunsetTime_
         << dayId
         << nightId;
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride::WriteUrban
// Created: NPT 2012-09-12
// -----------------------------------------------------------------------------
void PHY_Ephemeride::WriteUrban( xml::xostream& xos )
{
    std::string sunrise = boost::lexical_cast< std::string >( sunriseTime_.first ) + "h"
                        + boost::lexical_cast< std::string >( sunriseTime_.second ) + "m0s";
    std::string sunset  = boost::lexical_cast< std::string >( sunsetTime_.first ) + "h"
                        + boost::lexical_cast< std::string >( sunsetTime_.second ) + "m0s"; 
    xos << xml::attribute( "day-lighting", pDayBase_->GetName() )
        << xml::attribute( "night-lighting", pNightBase_->GetName() ) 
        << xml::attribute( "sunrise", sunrise )
        << xml::attribute( "sunset", sunset );
}

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::UpdateNight
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool PHY_Ephemeride::UpdateNight( unsigned int date )
{
    bpt::ptime pdate( bpt::from_time_t( date ) );
    bpt::time_duration time = pdate.time_of_day();
    std::pair< int, int > currentTime( time.hours(), time.minutes() );
    bool wasNight = bIsNight_;
    bIsNight_ = currentTime < sunriseTime_ || !( currentTime < sunsetTime_ );
    return bIsNight_ != wasNight;
}

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
const weather::PHY_Lighting& PHY_Ephemeride::GetLightingBase() const
{
    return bIsNight_ ? *pNightBase_ : *pDayBase_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
bool PHY_Ephemeride::IsNight() const
{
    return bIsNight_;
}
