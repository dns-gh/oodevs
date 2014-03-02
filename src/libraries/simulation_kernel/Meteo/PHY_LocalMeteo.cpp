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
#include "PHY_MeteoDataManager.h"
#include "RawVisionData/PHY_RawVisionData.h"
#include "Tools/MIL_MessageParameters.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "protocol/ClientSenders.h"
#include "protocol/EnumMaps.h"
#include "protocol/MessageParameters.h"
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
    : bIsPatched_( false )
    , startTime_ ( 0 )
    , endTime_   ( 0 )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, xml::xistream& xis,
        const weather::PHY_Lighting& light, unsigned int timeStep,
        const boost::shared_ptr< TER_World >& world )
    : Meteo( id, xis, &light, timeStep )
    , world_( world )
    , bIsPatched_( false )
    , startTime_ ( 0 )
    , endTime_   ( 0 )
{
    std::string strStartTime, strEndTime, strTopLeftPos, strTopRightPos;
    xis >> xml::attribute( "start-time", strStartTime )
        >> xml::attribute( "end-time", strEndTime )
        >> xml::attribute( "top-left", strTopLeftPos )
        >> xml::attribute( "bottom-right", strTopRightPos );
    world->MosToSimMgrsCoord( strTopLeftPos, upLeft_ );
    world->MosToSimMgrsCoord( strTopRightPos, downRight_ );
    startTime_ = ( bpt::from_iso_string( strStartTime ) - bpt::from_time_t( 0 ) ).total_seconds();
    endTime_ = ( bpt::from_iso_string( strEndTime ) - bpt::from_time_t( 0 ) ).total_seconds();
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: JSR 2010-04-12
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& msg,
        const weather::PHY_Lighting& light, unsigned int timeStep,
        const boost::shared_ptr< TER_World >& world )
    : Meteo( id, msg, light, timeStep )
    , world_( world )
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
void PHY_LocalMeteo::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    world_ = TER_World::GetWorldPtr();
    ar >> boost::serialization::base_object< weather::Meteo >( *this );
    ar >> startTime_;
    ar >> endTime_;
    ar >> upLeft_;
    ar >> downRight_;
    ar >> bIsPatched_;
}

void PHY_LocalMeteo::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< weather::Meteo >( *this );
    ar << startTime_;
    ar << endTime_;
    ar << upLeft_;
    ar << downRight_;
    ar << bIsPatched_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::Serialize
// Created: NPT 2012-09-10
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::Serialize( xml::xostream& xos ) const
{
    const std::string start = boost::posix_time::to_iso_string( bpt::from_time_t( startTime_ ) );
    const std::string end = boost::posix_time::to_iso_string( bpt::from_time_t( endTime_ ) );
    std::string coordUpLeft;
    std::string coordDownRight;
    world_->SimToMosMgrsCoord( upLeft_, coordUpLeft );
    world_->GetWorld().SimToMosMgrsCoord( downRight_, coordDownRight );
    xos << xml::attribute( "start-time", start )
        << xml::attribute( "end-time", end )
        << xml::attribute( "top-left", coordUpLeft )
        << xml::attribute( "bottom-right", coordDownRight );
    Meteo::Serialize( xos );
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
        const auto& start = protocol::GetDateTimeStr( msg, 7 );
        startTime_ = ( bpt::from_iso_string( start ) - bpt::from_time_t( 0 ) ).total_seconds();
        const auto& end = protocol::GetDateTimeStr( msg, 8 );
        endTime_ = ( bpt::from_iso_string( end ) - bpt::from_time_t( 0 ) ).total_seconds();
    }
    const auto& points = protocol::GetLocation( msg, 9 );
    protocol::Check( points.size() == 2u, "must have two points" );
    world_->MosToSimMgrsCoord( points[0].latitude(), points[0].longitude(), upLeft_ );
    world_->MosToSimMgrsCoord( points[1].latitude(), points[1].longitude(), downRight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::UpdateMeteoPatch( int date, PHY_RawVisionData& dataVision,
        const boost::shared_ptr< weather::Meteo >& meteo )
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
        dataVision.UnregisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_), geometry::Point2d( downRight_.rX_, downRight_.rY_ ), meteo );
        bIsPatched_ = false;
        modified_ = false;
        SendDestruction();
    }
    SendCreationIfModified();
}

namespace
{

void SimToWorld( const TER_World& w, MT_Vector2D sim, sword::CoordLatLong& world )
{
    double x, y;
    w.SimToMosMgrsCoord( sim, x, y );
    // While sword::CoordLatLong uses double, we convert to float first because
    // MeteoLocal uses float and using double gives a different model between
    // a running simulation and the dispatcher, hence different checkpoints.
    world.set_latitude( static_cast< float >( x ));
    world.set_longitude( static_cast< float >( y ));
}

} // namespace

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
    att->set_precipitation( protocol::ToProto( pPrecipitation_->GetID() ));
    att->set_temperature( temperature_ );
    att->set_lighting( protocol::ToProto( pLighting_->GetID() ));
    SimToWorld( *world_, downRight_, *msg().mutable_bottom_right() );
    SimToWorld( *world_, upLeft_, *msg().mutable_top_left() );
    msg().mutable_start_date()->set_data( bpt::to_iso_string( bpt::from_time_t( startTime_ ) ) );
    msg().mutable_end_date()->set_data( bpt::to_iso_string( bpt::from_time_t( endTime_ ) ) );
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

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::IsPatched
// Created: ABR 2012-03-21
// -----------------------------------------------------------------------------
bool PHY_LocalMeteo::IsPatched() const
{
    return bIsPatched_;
}
