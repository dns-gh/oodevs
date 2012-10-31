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
#include "UniqueId.h"
#include "SerializationTools.h"
#include "rpr/EntityType.h"
#include "rpr/Coordinates.h"
#include <hla/HLA_Types.h>

namespace plugins
{
namespace hla
{
namespace interactions
{

typedef VariableArray< NetnObjectDefinitionStruct > ListOfUnits;

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

// NETN Transfer of Modelling Responsability
struct TransactionId
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << transactionCounter;
        federateName.Serialize( archive );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> transactionCounter;
        federateName.Deserialize( archive );
    }
    uint32 transactionCounter;
    UnicodeString federateName;
};
struct TMR
{
    enum NoofferReasonEnum32
    {
        Reason_Other = 0,
        CapabilityDoesNotMatch = 1,
        AttributeSetToRestricted = 2,
        AttributeSetToExtensive = 3,
        FederateToBusy = 4,
        AttributeSetNotCompatibleWithPublication = 5
    };
    enum TransferTypeEnum32
    {
        Transfer_Other = 0,
        Acquire = 1,
        Divest = 2,
        AcquireWithoutNegotiating = 3
    };
    enum CapabilityTypeEnum32
    {
        Capability_Other = 0,
        TotalTransfer = 1,
        Movement = 2,
        Damage = 3,
        ResourceConsumption = 4
    };
    TransactionId transactionID;
    UnicodeString requestFederate;
    UnicodeString responseFederate;
};

struct TMR_OfferTransferModellingResponsibility : public TMR
{
    bool isOffering;
    uint32 reason; // type is NoofferReasonEnum32
};

struct AttributeValueStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        name.Serialize( archive );
        archive << static_cast< uint32 >( value.size() )
                << value;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        name.Deserialize( archive );
        uint32 sz = 0;
        archive >> sz;
        value.resize( sz );
        archive >> value;
    }
    UnicodeString name;
    std::vector< uint8 > value;
};

struct TMR_InitiateTransferModellingResponsibility : public TMR
{
    uint32 transferType; // TMR::TransferTypeEnum32
    VariableArray< NETN_UUID > instances;
    VariableArray< UnicodeString > attributes;
    UnicodeString intiating;
    uint32 capabilityType; // TMR::CapabilityTypeEnum32
    VariableArray< AttributeValueStruct >attributeValues;
};

struct TMR_RequestTransferModellingResponsibility : public TMR
{
    uint32 transferType; // TMR::TransferTypeEnum32
    VariableArray< NETN_UUID > instances;
    VariableArray< UnicodeString > attributes;
    uint32 capabilityType; // TMR::CapabilityTypeEnum32
    VariableArray< AttributeValueStruct > attributeValues;
};

} // namespace interactions
}
}

#endif // plugins_hla_interactions_InteractionsNetn_h
