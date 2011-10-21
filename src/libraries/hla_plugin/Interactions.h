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

struct NetnService
{
    NetnEventIdentifier serviceId;
    UnicodeString consumer;
    UnicodeString provider;
    int8 serviceType;
};

struct NetnRequestConvoy : public NetnService
{
    int64 requestTimeOut;
    NetnTransportStruct transportData;
};

struct NetnOfferConvoy : public NetnService
{
    int32 isOffering;
    int64 requestTimeOut;
    NetnTransportStruct transportData;
    int32 offerType;
    ListOfTransporters listOfTransporters;
};

struct NetnRejectOfferConvoy : public NetnService
{
    std::string reason;
};

struct NetnCancelConvoy : public NetnService
{
    std::string reason;
};

struct NetnAcceptOffer : public NetnService
{};

struct NetnReadyToReceiveService : public NetnService
{};

struct NetnServiceStarted : public NetnService
{};

struct NetnConvoyEmbarkmentStatus : public NetnService
{
    ListOfTransporters listOfObjectEmbarked;
    UnicodeString transportUnitIdentifier;
};

struct NetnConvoyDisembarkmentStatus : public NetnService
{
    ListOfTransporters listOfObjectDisembarked;
    UnicodeString transportUnitIdentifier;
};

struct NetnConvoyDestroyedEntities : public NetnService
{
    ListOfTransporters listOfEmbarkedObjectDestroyed;
};

struct NetnServiceComplete : public NetnService
{};

struct NetnServiceReceived : public NetnService
{};

}
}
}

#endif // plugins_hla_interactions_Interactions_h
