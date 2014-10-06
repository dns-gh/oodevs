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
#include <cstring>

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
    bool operator==(const TransactionId& rhs) const
    {
        if( transactionCounter != rhs.transactionCounter || federateHandle.Size() != rhs.federateHandle.Size() )
            return false;

        const uint8_t* b1 = federateHandle.Buffer();
        const uint8_t* b2 = rhs.federateHandle.Buffer();
        return std::memcmp( b1, b2, federateHandle.Size() ) == 0;
    }
    bool operator<(const TransactionId& rhs) const
    {
        if( transactionCounter < rhs.transactionCounter )
            return true;
        if( transactionCounter > rhs.transactionCounter )
            return false;
        if( federateHandle.Size() < rhs.federateHandle.Size() )
            return true;
        if( federateHandle.Size() > rhs.federateHandle.Size() )
            return false;

        const uint8_t* b1 = federateHandle.Buffer();
        const uint8_t* b2 = rhs.federateHandle.Buffer();
        return std::memcmp( b1, b2, federateHandle.Size() ) < 0;
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
    PaddedVariableArray< UnicodeString, 4 > attributes;
    uint32_t capabilityType; // TMR::CapabilityTypeEnum32
    VariableArray< AttributeValueStruct >attributeValues;
};

struct TMR_RequestTransferModellingResponsibility : public TMR
{
    uint32_t transferType; // TMR::TransferTypeEnum32
    VariableArray< NETN_UUID > instances;
    PaddedVariableArray< UnicodeString, 4 > attributes;
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
    uint32_t transferOk; // 1 : OK, 0 : KO
};

struct MRM_Object
{
    enum NonComplianceReasonEnum
    {
        noncompliancereasonenum_Other = 0,
        noncompliancereasonenum_UnitUnknown = 1,
        noncompliancereasonenum_UnitAlreadyDisaggregated = 2,
        noncompliancereasonenum_UnitAlreadyAggregated = 3,
        noncompliancereasonenum_UnitNotOwned = 4
    };
    enum ResponseFlagEnum16
    {
        responseflagenum16_Other = 0,
        responseflagenum16_AbleToComply = 1,
        responseflagenum16_UnableToComply = 2
    };
    TransactionId transactionID;
    UnicodeString aggregateFederate;
    UnicodeString higherResolutionFederate;
};

struct MRM_DisaggregationRequest : public MRM_Object
{
    NETN_UUID aggregateUUID;
    uint32_t aggregationState;
    VariableArray< NETN_UUID > uuidsList;
};

struct MRM_DisaggregationResponse : public MRM_Object
{
    uint16_t acknowledge;
    uint32_t nonComplianceReason;
};

struct MRM_AggregationRequest : public MRM_Object
{
    NETN_UUID aggregateUUID;
    VariableArray< NETN_UUID > uuidsList;
};

struct MRM_AggregationResponse : public MRM_Object
{
    uint16_t acknowledge;
    uint32_t nonComplianceReason; // NonComplianceReasonEnum
};

struct MRM_CancelRequest : public MRM_Object
{
    enum CancellationReasonEnum32
    {
        cancellationReasonEnum32_Other = 0,
        cancellationReasonEnum32TimeOut = 1
    };
    uint32_t reason;
};

struct MRM_ActionComplete : public MRM_Object
{
    uint32_t result; // 1 : OK, 0 : KO
};

struct MRM_Trigger
{
    NETN_UUID instance;
    uint32_t aggregationState;
    VariableArray< NETN_UUID > uuidList;
};

struct MRM_TriggerResponse
{
    NETN_UUID instance;
    TransactionId transactionID;
};

} // namespace interactions
}
}

#endif // plugins_hla_interactions_InteractionsNetn_h
