// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_GlobalMeteo.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Lighting.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/EnumMaps.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_GlobalMeteo )

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo( xml::xistream& xis, const weather::PHY_Lighting& light, unsigned int timeStep )
    : Meteo( 1, xis, &light, timeStep )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo destructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::~PHY_GlobalMeteo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo::SendRegister
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::SendCreation() const
{
    client::ControlGlobalWeather msg;
    msg().mutable_weather()->set_id( id_ );
    sword::WeatherAttributes* att = msg().mutable_attributes();
    att->set_wind_speed( static_cast< int >( wind_.rSpeed_ / conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( wind_.eAngle_ );
    att->set_cloud_floor( cloud_.nFloor_ );
    att->set_cloud_ceiling( cloud_.nCeiling_ );
    att->set_cloud_density( cloud_.nDensityPercentage_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( temperature_ );
    att->set_lighting( protocol::ToProtoLighting( GetLighting().GetID() ));
    msg.Send( NET_Publisher_ABC::Publisher() );
}
