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
#include <hla/FederateIdentifier.h>

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
    int8_t serviceType;
};

struct NetnRequestConvoy : public NetnService
{
    int64_t requestTimeOut;
    NetnTransportStruct transportData;
};

struct NetnOfferConvoy : public NetnService
{
    int32_t isOffering;
    int64_t requestTimeOut;
    NetnTransportStruct transportData;
    int32_t offerType;
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
        const uint32_t nameSize = static_cast<uint32_t>(federateHandle.Size());
        archive << nameSize;
        const uint8_t* b = federateHandle.Buffer();
        for( std::size_t i = 0; i < nameSize; ++i )
            archive << *(b+i);
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32_t nameSize;
        archive >> transactionCounter
                >> nameSize;
        std::vector< uint8_t > b(nameSize,0);
        archive >> b;
        federateHandle = ::hla::FederateIdentifier( &b[0], nameSize );
    }
    uint32_t transactionCounter;
    ::hla::FederateIdentifier federateHandle;
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
    UnicodeString respondent;
    uint32_t reason; // type is NoofferReasonEnum32
};

struct AttributeValueStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        name.Serialize( archive );
        archive << static_cast< uint32_t >( value.size() )
                << value;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        name.Deserialize( archive );
        uint32_t sz = 0;
        archive >> sz;
        value.resize( sz );
        archive >> value;
    }
    UnicodeString name;
    std::vector< uint8_t > value;
};

struct TMR_InitiateTransferModellingResponsibility : public TMR
{
    UnicodeString intiating;
    uint32_t transferType; // TMR::TransferTypeEnum32
    VariableArray< NETN_UUID > instances;
    VariableArray< UnicodeString > attributes;
    uint32_t capabilityType; // TMR::CapabilityTypeEnum32
    VariableArray< AttributeValueStruct >attributeValues;
};

struct TMR_RequestTransferModellingResponsibility : public TMR
{
    uint32_t transferType; // TMR::TransferTypeEnum32
    VariableArray< NETN_UUID > instances;
    VariableArray< UnicodeString > attributes;
    uint32_t capabilityType; // TMR::CapabilityTypeEnum32
    VariableArray< AttributeValueStruct > attributeValues;
};

struct TMR_CancelRequest : public TMR
{
    enum CancellationReasonEnum32
    {
        Other = 0,
        TimeOut = 1
    };
    uint32_t reason;
};

struct TMR_TransferResult : public TMR
{
    bool transferOk;
};

} // namespace interactions
}
}

#endif // plugins_hla_interactions_InteractionsNetn_h
