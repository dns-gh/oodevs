// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_ENUM_MAPS
#define PROTOCOL_ENUM_MAPS

#include "ENT/ENT_Enums_Gen.h"
#pragma warning( push, 0 )
#include "protocol/proto/simulation_client.pb.h"
#pragma warning( pop )

namespace protocol
{

E_LightingType FromProto( sword::WeatherAttributes::EnumLightingType value );
sword::WeatherAttributes::EnumLightingType ToProto( E_LightingType value );

E_WeatherType FromProto( sword::WeatherAttributes::EnumPrecipitationType value );
sword::WeatherAttributes::EnumPrecipitationType ToProto( E_WeatherType value );

}  // protocol

#endif // PROTOCOL_ENUM_MAPS
