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
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ClientSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Lighting.h"

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo( unsigned int id, xml::xistream& xis, const weather::PHY_Lighting& light, int conversionFactor )
    : PHY_Meteo( id, xis, light, conversionFactor )
    , isChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo constructor
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
PHY_GlobalMeteo::PHY_GlobalMeteo( unsigned int id, const Common::MsgMeteoAttributes& msg, weather::MeteoManager_ABC* list )
    : PHY_Meteo( id, msg, list )
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
// Name: PHY_GlobalMeteo::Update
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::Update( const Common::MsgMissionParameters& asn )
{
    isChanged_ = true;
    PHY_Meteo::Update( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo::UpdateMeteoPatch
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::UpdateMeteoPatch( int /*date*/, weather::PHY_RawVisionData_ABC& /*dataVision*/ )
{
    if( isChanged_ )
    {
        isChanged_ = false;
        SendCreation();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_GlobalMeteo::SendRegister
// Created: HBD 2010-03-25
// -----------------------------------------------------------------------------
void PHY_GlobalMeteo::SendCreation() const
{
    client::ControlGlobalMeteo msg;
    msg().set_oid( id_ );

    Common::MsgMeteoAttributes* att = msg().mutable_attributes();
    att->set_wind_speed( int( wind_.rWindSpeed_ / conversionFactor_ ) );
    NET_ASN_Tools::WriteDirection( wind_.vWindDirection_, *( att->mutable_wind_direction() ) );
    att->set_cloud_floor( nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( int( rDensiteCouvertureNuageuse_ * 100. + 0.01 ) );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    att->set_lighting( GetLighting().GetAsnID() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

