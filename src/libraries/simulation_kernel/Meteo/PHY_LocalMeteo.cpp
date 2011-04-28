// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_LocalMeteo.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light, int conversionFactor )
    : PHY_Meteo( id, xis, light, conversionFactor )
    , bIsPatched_( false )
{
    std::string strStartTime, strEndTime, strTopLeftPos, strTopRightPos;
    xis >> xml::attribute( "start-time", strStartTime )
        >> xml::attribute( "end-time", strEndTime )
        >> xml::attribute( "top-left", strTopLeftPos )
        >> xml::attribute( "bottom-right", strTopRightPos );
    MIL_Tools::ConvertCoordMosToSim( strTopLeftPos, upLeft_ );
    MIL_Tools::ConvertCoordMosToSim( strTopRightPos, downRight_ );
    startTime_ = ( bpt::from_iso_string( strStartTime ) - bpt::from_time_t( 0 ) ).total_seconds();
    endTime_ = ( bpt::from_iso_string( strEndTime ) - bpt::from_time_t( 0 ) ).total_seconds();
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: JSR 2010-04-12
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& msg, weather::MeteoManager_ABC* list )
    : PHY_Meteo( id, msg, list )
    , bIsPatched_( false )
{
    const sword::MissionParameter& startTime = msg.elem( 7 );
    if( startTime.value_size() != 1 || !startTime.value().Get(0).has_datetime() )
        throw std::exception( "Meteo : bad attribute for StartTime" );
    startTime_ = ( bpt::from_iso_string( startTime.value().Get(0).datetime().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
    const sword::MissionParameter& endTime = msg.elem( 8 );
    if( endTime.value_size() != 1 || !endTime.value().Get(0).has_datetime() )
        throw std::exception( "Meteo : bad attribute for EndTime" );
    endTime_ = ( bpt::from_iso_string( endTime.value().Get(0).datetime().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
    const sword::MissionParameter& location = msg.elem( 9 );
    if( location.value_size() != 1 || !location.value().Get(0).has_location() )
        throw std::exception( "Meteo : bad attribute for Location" );
    NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 0 ), upLeft_    );
    NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 1 ), downRight_ );
    isChanged_ = false;
}


// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo destructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::~PHY_LocalMeteo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::UpdateMeteoPatch( int date, weather::PHY_RawVisionData_ABC& dataVision )
{
    bool bNeedToBePatched = ( date > startTime_ &&  date < endTime_ );
    if( !bIsPatched_ && bNeedToBePatched )
    {
        dataVision.RegisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = true;
        isChanged_ = true;
    }
    else if( bIsPatched_ && !bNeedToBePatched )
    {
        dataVision.UnregisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = false;
        isChanged_ = false;
        SendDestruction();
    }
    PHY_Meteo::UpdateMeteoPatch( date, dataVision );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::SendRegister
// Created: HBD 2010-03-24
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::SendCreation() const
{
    client::ControlLocalWeatherCreation msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rWindSpeed_ / conversionFactor_ ) );
    NET_ASN_Tools::WriteDirection(wind_.vWindDirection_, *(att->mutable_wind_direction()) );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( static_cast< int >( rDensiteCouvertureNuageuse_ * 100. + 0.01 ) );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    att->set_lighting( pLighting_->GetAsnID() );
    sword::CoordLatLong longlat;
    MIL_Tools::ConvertCoordSimToMos( downRight_, longlat);
    msg().mutable_bottom_right()->set_latitude( longlat.latitude()  );
    msg().mutable_bottom_right()->set_longitude( longlat.longitude()  );
    MIL_Tools::ConvertCoordSimToMos( upLeft_, longlat);
    msg().mutable_top_left()->set_latitude( longlat.latitude()  );
    msg().mutable_top_left()->set_longitude( longlat.longitude()  );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::SendDestruction
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::SendDestruction()
{
    client::ControlLocalWeatherDestruction msg;
    msg().mutable_weather()->set_id( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}
