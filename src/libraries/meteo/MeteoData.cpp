// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MeteoData.h"
#include "MeteoModel_ABC.h"
#include "PHY_Lighting.h"
#include "PHY_Precipitation.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/ClientPublisher_ABC.h"

using namespace weather;

// -----------------------------------------------------------------------------
// Name: MeteoData constructor
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
MeteoData::MeteoData( unsigned int id, const geometry::Point2f& upLeft, const geometry::Point2f& downRight, const sword::WeatherAttributes& attributes, MeteoModel_ABC& model, kernel::CoordinateConverter_ABC& converter, unsigned int timeStep )
    : PHY_Meteo( id, attributes, &model, timeStep )
    , converter_( converter )
    , rect_     ( upLeft.X(), downRight.Y(), downRight.X(), upLeft.Y() )
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
void MeteoData::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlLocalWeatherCreation msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rWindSpeed_ / PHY_Meteo::conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( 0 );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( static_cast< int >( rDensiteCouvertureNuageuse_ ) );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    att->set_lighting( pLighting_->GetAsnID() );

    geometry::Point2f downRight( rect_.Right(), rect_.Bottom() );
    geometry::Point2d latlong = converter_.ConvertToGeo( downRight );
    msg().mutable_bottom_right()->set_longitude( latlong.X() );
    msg().mutable_bottom_right()->set_latitude( latlong.Y() );

    geometry::Point2f upLeft( rect_.Left(), rect_.Top() );
    latlong = converter_.ConvertToGeo( upLeft );
    msg().mutable_top_left()->set_longitude( latlong.X() );
    msg().mutable_top_left()->set_latitude( latlong.Y() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: MeteoData::SendDestruction
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void MeteoData::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlLocalWeatherDestruction msg;
    msg().mutable_weather()->set_id( id_ );
    msg.Send( publisher );
}
