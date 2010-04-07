// *****************************************************************************
//q
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MeteoData.h"
#include "MeteoModel_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "PHY_Precipitation.h"
#include "PHY_Lighting.h"

// -----------------------------------------------------------------------------
// Name: MeteoData constructor
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
MeteoData::MeteoData( unsigned int id, const geometry::Point2f& upLeft, const geometry::Point2f& downRight, const Common::MsgMeteoAttributes& att, MeteoModel_ABC* listener )
    : PHY_Meteo( id, att, listener )
    , rect_ ( upLeft.X(), downRight.Y(), downRight.X(), upLeft.Y() )
    , listener_( *listener )
{
    // NOTHING
  
}

// -----------------------------------------------------------------------------
// Name: MeteoData destructor
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
MeteoData::~MeteoData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoData::IsInside
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
bool MeteoData::IsInside( const geometry::Point2f& point ) const
{
    return rect_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: MeteoData::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void MeteoData::SendCreation( dispatcher::ClientPublisher_ABC& publisher) const
{
    client::ControlLocalMeteoCreation msg;
    Common::MsgMeteoAttributes* att = msg().mutable_attributes();
    msg().set_oid( id_ );
    att->set_wind_speed(  wind_.rWindSpeed_ / conversionFactor_ );
    att->mutable_wind_direction()->set_heading( 0 );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( rDensiteCouvertureNuageuse_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );

    geometry::Point2f downRight( rect_.Right(), rect_.Bottom() );
    geometry::Point2d latlong = listener_.GetConverter().ConvertToGeo( downRight );
    msg().mutable_bottom_right_coordinate()->set_longitude( latlong.X() );
    msg().mutable_bottom_right_coordinate()->set_latitude( latlong.Y() );

    geometry::Point2f upLeft( rect_.Left(), rect_.Top() );
    latlong = listener_.GetConverter().ConvertToGeo( upLeft );
    msg().mutable_top_left_coordinate()->set_longitude( latlong.X() );
    msg().mutable_top_left_coordinate()->set_latitude( latlong.Y() );
    msg.Send( publisher );      
}

void MeteoData::SendDestruction( dispatcher::ClientPublisher_ABC& publisher) const
{
    client::ControlLocalMeteoDestruction msg;
    msg().set_oid( id_ );
    msg.Send( publisher );    
}