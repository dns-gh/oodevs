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
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/MeteoManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Meteo/PHY_RawVisionData_ABC.h"
#include "meteo/PHY_Precipitation.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.h>

#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, xml::xistream& xis, const PHY_Lighting& light, int conversionFactor )
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

//-----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( unsigned int id, const MsgsClientToSim::MsgControlLocalMeteo& msg, MeteoManager_ABC* list )
    : PHY_Meteo( id, msg.attributes(), list )
{
    NET_ASN_Tools::ReadPoint( msg.top_left_coordinate(),      upLeft_    );
    NET_ASN_Tools::ReadPoint( msg.bottom_right_coordinate() , downRight_ );
    startTime_ = ( bpt::from_iso_string( msg.start_time().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
    endTime_ = ( bpt::from_iso_string( msg.end_time().data() ) - bpt::from_time_t( 0 ) ).total_seconds();
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo destructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::~PHY_LocalMeteo()
{
    //NTOHING
}


// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision_ )
{
    bool bNeedToBePatched = ( date > startTime_ &&  date < endTime_ );
    if( !bIsPatched_ && bNeedToBePatched )
    {
        dataVision_.RegisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = true;
        SendCreation();
    }
    else if( bIsPatched_ && !bNeedToBePatched )
    {
        dataVision_.UnregisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = false;
        SendDestruction();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::SendRegister
// Created: HBD 2010-03-24
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::SendCreation() const
{
    client::ControlLocalMeteoCreation msg;
    Common::MsgMeteoAttributes* att = msg().mutable_attributes();
    msg().set_oid( id_ );

    att->set_wind_speed(  wind_.rWindSpeed_ / conversionFactor_ );
    NET_ASN_Tools::WriteDirection(wind_.vWindDirection_, *(att->mutable_wind_direction()) );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( rDensiteCouvertureNuageuse_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );

    Common::MsgCoordLatLong longlat;
    MIL_Tools::ConvertCoordSimToMos( downRight_, longlat);
    msg().mutable_bottom_right_coordinate()->set_latitude( longlat.latitude()  );
    msg().mutable_bottom_right_coordinate()->set_longitude( longlat.longitude()  );

    MIL_Tools::ConvertCoordSimToMos( upLeft_, longlat);
    msg().mutable_top_left_coordinate()->set_latitude( longlat.latitude()  );
    msg().mutable_top_left_coordinate()->set_longitude( longlat.longitude()  );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

void PHY_LocalMeteo::SendDestruction() 
{
    client::ControlLocalMeteoDestruction msg;
    msg().set_oid( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}
