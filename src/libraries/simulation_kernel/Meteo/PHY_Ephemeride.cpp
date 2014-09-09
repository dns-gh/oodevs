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
#include "meteo/PHY_Lighting.h"
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/make_shared.hpp>

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

namespace
{

bool ParseTime( const std::string& s, std::pair< int, int >& result )
{
    char sep = 0;
    std::istringstream stream( s );
    stream >> result.first >> sep >> result.second;
    return sep == 'h'
        && result.first >= 0 && result.first <= 23
        && result.second >= 0 && result.second <= 59;
}

}  // namespace

boost::shared_ptr< PHY_Ephemeride > ReadEphemeride(
        xml::xistream& xis, uint32_t epochTime )
{
    xis >> xml::start( "ephemerides" );
    std::string sunRise = xis.attribute< std::string >( "sunrise" );
    std::string sunSet = xis.attribute< std::string >( "sunset" );
    std::string dayBase, nightBase;
    if( xis.has_attribute( "moon" ) )
    {
        dayBase = weather::PHY_Lighting::jourSansNuage_.GetName();
        nightBase = xis.attribute< std::string >( "moon" );
    }
    else
    {
        dayBase = xis.attribute< std::string >( "day-lighting" );
        nightBase = xis.attribute< std::string >( "night-lighting" );
    }
    xis >> xml::end;
    return boost::make_shared< PHY_Ephemeride >(
            dayBase, nightBase, sunRise, sunSet, epochTime );
}

PHY_Ephemeride::PHY_Ephemeride(
        const std::string& dayBase, const std::string& nightBase,
        const std::string& sunrise, const std::string& sunset, uint32_t epochTime )
    : bIsNight_( false )
{
    pDayBase_ = weather::PHY_Lighting::FindLighting( dayBase );
    if( !pDayBase_ )
        throw MASA_EXCEPTION( "invalid day lighting: " + dayBase );
    pNightBase_ = weather::PHY_Lighting::FindLighting( nightBase );
    if( !pNightBase_ )
        throw MASA_EXCEPTION( "invalid night lighting: " + nightBase );

    if( !ParseTime( sunrise, sunriseTime_ ))
        throw MASA_EXCEPTION( "invalid sunrise time, use 00h00" );
    if( !ParseTime( sunset, sunsetTime_ ))
        throw MASA_EXCEPTION( "invalid sunset time, use 00h00" );
    if( sunriseTime_ >= sunsetTime_  )
        throw MASA_EXCEPTION( "sunrise time should be before sunset time" );
    UpdateNight( epochTime );
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
    file >> bIsNight_;
    file >> sunriseTime_;
    file >> sunsetTime_;
    file >> dayId;
    file >> nightId;
    pDayBase_ = weather::PHY_Lighting::FindLighting( static_cast< E_LightingType >( dayId ) );
    pNightBase_ = weather::PHY_Lighting::FindLighting( static_cast< E_LightingType >( nightId ));
}

// -----------------------------------------------------------------------------
// Name: PHY_Ephemeride::save
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_Ephemeride::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned int dayId = pDayBase_->GetID();
    unsigned int nightId = pNightBase_->GetID();
    file << bIsNight_;
    file << sunriseTime_;
    file << sunsetTime_;
    file << dayId;
    file << nightId;
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
bool PHY_Ephemeride::UpdateNight( uint32_t date )
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
