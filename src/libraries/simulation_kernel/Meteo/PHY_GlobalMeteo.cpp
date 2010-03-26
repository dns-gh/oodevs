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
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "protocol/ClientSenders.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Lighting.h"

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo(  xml::xistream& xis, const PHY_Lighting& light, int conversionFactor )
    : PHY_Meteo( xis, light, conversionFactor )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo( const Common::MsgMeteoAttributes& msg, MeteoManager_ABC* list )
    : PHY_Meteo( msg, list )
{

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
// Name: PHY_GlobalMeteo::SendRegisterLocal
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::SendRegisterGlobal()
{
    client::ControlGlobalMeteo msg;
    Common::MsgMeteoAttributes* att = msg().mutable_attributes();

    att->set_wind_speed(  wind_.rWindSpeed_ / conversionFactor_ );
    NET_ASN_Tools::WriteDirection(wind_.vWindDirection_, *(att->mutable_wind_direction()) );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( rDensiteCouvertureNuageuse_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo::SendUnregisterGlobal
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::SendUnregisterGlobal()
{
    client::ControlGlobalMeteo msg;
    Common::MsgMeteoAttributes* att = msg().mutable_attributes();

    att->set_wind_speed(  wind_.rWindSpeed_ / conversionFactor_ );
    NET_ASN_Tools::WriteDirection(wind_.vWindDirection_, *(att->mutable_wind_direction()) );
    att->set_cloud_floor (nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( rDensiteCouvertureNuageuse_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );

    msg.Send( NET_Publisher_ABC::Publisher() );
}

