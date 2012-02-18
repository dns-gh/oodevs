// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_interactions_InteractionsRpr_h
#define plugins_hla_interactions_InteractionsRpr_h

#include "Omt13String.h"
#include "Transportation.h"
#include "UnicodeString.h"
#include "rpr/EntityIdentifier.h"
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
    std::vector< int8 > articulatedPartData;
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

struct SupplyStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << supplyType
                << quantity;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> supplyType
                >> quantity;
    }

    rpr::EntityType supplyType;
    real32 quantity;
};

struct ServiceRequest
{
    enum ServiceTypeEnum
    {
        Other       = 0,
        Resupply    = 1,
        Repair      = 2
    };

    Omt13String receivingObject;
    Omt13String servicingObject;
    uint8 serviceType;
    std::vector< SupplyStruct > suppliesData;
};

struct ResupplyCancel
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
};

struct ResupplyOffer
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
    std::vector< SupplyStruct > suppliesData;
};

struct ResupplyReceived
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
    std::vector< SupplyStruct > suppliesData;
};

struct RecordSet
{
    struct RecordStruct
    {
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            int32 size = data.size();
            archive << numberOfBits
                    << size
                    << data;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            int32 size = 0;
            archive >> numberOfBits
                    >> size;
            data.resize( size );
            archive >> data;
        }
        uint32 numberOfBits;
        std::vector<char> data;
    };
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        int32 size = values.size();
        archive << recordSetIdentifier
                << recordSetSerialNumber
                << size
                << values;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int32 size = 0;
        archive >> recordSetIdentifier
                >> recordSetSerialNumber
                >> size;
        values.resize( size );
        archive >> values;
    }
    uint32 recordSetIdentifier;
    uint32 recordSetSerialNumber;
    std::vector<RecordStruct> values;
};

struct TransferControl
{
    rpr::EntityIdentifier originatingEntity;
    rpr::EntityIdentifier receivingEntity;
    uint32 requestIdentifier;
    uint8 transferType;
    Omt13String transferEntity;
    std::vector<RecordSet> recordSetData;
};

}
}
}

#endif // plugins_hla_interactions_InteractionsRpr_h
