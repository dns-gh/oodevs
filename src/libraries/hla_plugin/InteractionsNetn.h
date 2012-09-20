// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_interactions_InteractionsNetn_h
#define plugins_hla_interactions_InteractionsNetn_h

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

struct ListOfUnits
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        int32 size = static_cast< int32 >( list.size() );
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
    ListOfUnits listOfTransporters;
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
    ListOfUnits listOfObjectEmbarked;
    UnicodeString transportUnitIdentifier;
};

struct NetnConvoyDisembarkmentStatus : public NetnService
{
    ListOfUnits listOfObjectDisembarked;
    UnicodeString transportUnitIdentifier;
};

struct NetnConvoyDestroyedEntities : public NetnService
{
    ListOfUnits listOfEmbarkedObjectDestroyed;
};

struct NetnServiceComplete : public NetnService
{};

struct NetnServiceReceived : public NetnService
{};

}
}
}

#endif // plugins_hla_interactions_InteractionsNetn_h
