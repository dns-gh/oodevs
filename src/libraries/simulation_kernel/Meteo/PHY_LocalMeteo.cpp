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
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "MIL_AgentServer.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_LocalMeteo )

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light, unsigned int timeStep )
    : Meteo( id, xis, &light, timeStep )
    , bIsPatched_( false )
    , startTime_ ( 0 )
    , endTime_   ( 0 )
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
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& msg, const weather::PHY_Lighting& light, unsigned int timeStep )
    : Meteo( id, msg, light, timeStep )
    , bIsPatched_( false )
    , startTime_ ( 0 )
    , endTime_   ( 0 )
{
    LocalUpdate( msg, true );
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
// Name: PHY_LocalMeteo::serialize
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_LocalMeteo::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< weather::Meteo >( *this )
         & startTime_
         & endTime_
         & upLeft_
         & downRight_
         & bIsPatched_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::Update
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::Update( const sword::MissionParameters& msg )
{
    Meteo::Update( msg );
    LocalUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::LocalUpdate
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::LocalUpdate( const sword::MissionParameters& msg, bool isCreation /*= false*/ )
{
    if( isCreation )
    {
        const sword::MissionParameter& startTime = msg.elem( 7 );
        if( startTime.value_size() != 1 || !startTime.value().Get(0).has_datetime() )
            throw std::exception( "Meteo : bad attribute for StartTime" );
        startTime_ = ( bpt::from_iso_string( startTime.value().Get(0).datetime().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
        const sword::MissionParameter& endTime = msg.elem( 8 );
        if( endTime.value_size() != 1 || !endTime.value().Get(0).has_datetime() )
            throw std::exception( "Meteo : bad attribute for EndTime" );
        endTime_ = ( bpt::from_iso_string( endTime.value().Get(0).datetime().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
    }
    const sword::MissionParameter& location = msg.elem( 9 );
    if( location.value_size() != 1 || !location.value().Get(0).has_location() )
        throw std::exception( "Meteo : bad attribute for Location" );
    if( location.value().Get(0).location().coordinates().elem_size() == 2 ) // $$$$ ABR 2011-09-29: Sword protocol
    {
        NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 0 ), upLeft_    );
        NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 1 ), downRight_ );
    }
    else if( location.value().Get(0).location().coordinates().elem_size() == 4 ) // $$$$ ABR 2011-09-29: Shield protocol
    {
        NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 0 ), upLeft_    );
        NET_ASN_Tools::ReadPoint( location.value().Get(0).location().coordinates().elem( 2 ), downRight_ );
    }
    else
        throw std::exception( "Meteo : bad attribute for Location" );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::UpdateMeteoPatch( int date, weather::PHY_RawVisionData_ABC& dataVision, boost::shared_ptr< weather::Meteo > meteo )
{
    bool bNeedToBePatched = ( date > startTime_ && date < endTime_ );
    if( !bIsPatched_ && bNeedToBePatched )
    {
        dataVision.RegisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_), geometry::Point2d( downRight_.rX_, downRight_.rY_ ), meteo );
        bIsPatched_ = true;
        modified_ = true;
    }
    else if( bIsPatched_ && !bNeedToBePatched )
    {
        dataVision.UnregisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_ ) );
        bIsPatched_ = false;
        modified_ = false;
        SendDestruction();
    }
    Meteo::UpdateMeteoPatch( date, dataVision, meteo );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::SendRegister
// Created: HBD 2010-03-24
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::SendCreation() const
{
    if( !bIsPatched_ )
        return;
    client::ControlLocalWeatherCreation msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rSpeed_ / conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( wind_.eAngle_ );
    att->set_cloud_floor ( cloud_.nFloor_ );
    att->set_cloud_ceiling( cloud_.nCeiling_ );
    att->set_cloud_density( cloud_.nDensityPercentage_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( temperature_ );
    att->set_lighting( pLighting_->GetAsnID() );
    sword::CoordLatLong longlat;
    MIL_Tools::ConvertCoordSimToMos( downRight_, longlat );
    msg().mutable_bottom_right()->set_latitude( longlat.latitude()  );
    msg().mutable_bottom_right()->set_longitude( longlat.longitude()  );
    MIL_Tools::ConvertCoordSimToMos( upLeft_, longlat );
    msg().mutable_top_left()->set_latitude( longlat.latitude()  );
    msg().mutable_top_left()->set_longitude( longlat.longitude()  );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::SendDestruction
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::SendDestruction() const
{
    client::ControlLocalWeatherDestruction msg;
    msg().mutable_weather()->set_id( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}
