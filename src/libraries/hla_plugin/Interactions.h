// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_interactions_Interactions_h
#define plugins_hla_interactions_Interactions_h

#include "Omt13String.h"
#include "Transportation.h"
#include "UnicodeString.h"
#include "rpr/EntityType.h"
#include "rpr/Coordinates.h"
#include <hla/HLA_Types.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
struct EventIdentifierStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << eventCount;
        issuingObjectIdentifier.Serialize( archive );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> eventCount;
        issuingObjectIdentifier.Deserialize( archive );
    }
    uint16 eventCount;
    Omt13String issuingObjectIdentifier;
};

struct ListOfTransporters
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        int32 size = list.size();
        archive << size
                << list;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int32 size = 0;
        archive >> size;
        list.resize( size );
        archive >> list;
    }
    std::vector< NetnObjectDefinitionStruct > list;
};

struct MunitionDetonation
{
    uint32 articulatedPartData;
    rpr::WorldLocation detonationLocation;
    int8 detonationResultCode;
    EventIdentifierStruct eventIdentifier;
    Omt13String firingObjectIdentifier;
    rpr::VelocityVector finalVelocityVector;
    int16 fuseType;
    Omt13String munitionObjectIdentifier;
    rpr::EntityType munitionType;
    uint16 quantityFired;
    uint16 rateOfFire;
    rpr::VelocityVector relativeDetonationLocation; // RelativePositionStruct
    Omt13String targetObjectIdentifier;
    int16 warheadType;
};

struct NetnRequestConvoy
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
    int64 requestTimeOut;
    NetnTransportStruct transportData;
};

struct NetnOfferConvoy
{
    int32 isOffering;
    int64 requestTimeOut;
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
    NetnTransportStruct transportData;
    int32 offerType;
    ListOfTransporters listOfTransporters;
};

struct NetnRejectOfferConvoy
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
    std::string reason;
};

struct NetnAcceptOffer
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
};

struct NetnReadyToReceiveService
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
};

struct NetnServiceStarted
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
};

struct NetnConvoyEmbarkmentStatus
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
    ListOfTransporters listOfObjectEmbarked;
    UnicodeString transportUnitIdentifier;
};
}
}
}

#endif // plugins_hla_interactions_Interactions_h
