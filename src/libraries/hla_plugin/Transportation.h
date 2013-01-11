// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Transportation_h
#define plugins_hla_Transportation_h

#include "UnicodeString.h"
#include "UniqueId.h"
#include "SerializationTools.h"
#include "rpr/Coordinates.h"
#include "rpr/EntityType.h"
#include <hla/HLA_Types.h>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  NetnObjectDescription
    @brief  Netn object description
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnObjectDescription
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnObjectDescription();
             NetnObjectDescription( float weight, float volume, const std::string& type );
    virtual ~NetnObjectDescription();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << weight
                << volume
                << type;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> weight
                >> volume
                >> type;
    }
    //@}

    //! @name Member data
    //@{
    real32 weight;
    real32 volume;
    std::string type;
    //@}
};

// =============================================================================
/** @class  NetnHumanDescription
    @brief  Netn human description
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnHumanDescription
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnHumanDescription();
             NetnHumanDescription( const rpr::EntityType& humanType, short quantity, short injury );
    virtual ~NetnHumanDescription();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << humanType
                << quantity
                << injury;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> humanType
                >> quantity
                >> injury;
    }
    //@}

    //! @name Member data
    //@{
    rpr::EntityType humanType;
    int16 quantity;
    int16 injury;
    //@}
};

// =============================================================================
/** @class  NetnEquipDescription
    @brief  Netn equipment description
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnEquipDescription
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnEquipDescription();
             NetnEquipDescription( const rpr::EntityType& equipType, int quantity, int damageState );
    virtual ~NetnEquipDescription();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << equipType
                << quantity
                << damageState;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> equipType
                >> quantity
                >> damageState;
    }
    //@}

    //! @name Member data
    //@{
    rpr::EntityType equipType;
    int32 quantity;
    int32 damageState;
    //@}
};

// =============================================================================
/** @class  NetnPlateformDescription
    @brief  Netn plateform description
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnPlateformDescription
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnPlateformDescription();
             NetnPlateformDescription( const rpr::EntityType& plateformType, int damageState );
    virtual ~NetnPlateformDescription();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << plateformType
                << damageState;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> plateformType
                >> damageState;
    }
    //@}

    //! @name Member data
    //@{
    rpr::EntityType plateformType;
    int32 damageState;
    //@}
};

class NetnObjectDefinitionStruct;

// =============================================================================
/** @class  NetnObjectFeatureStruct
    @brief  Netn object feature struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnObjectFeatureStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnObjectFeatureStruct();
    explicit NetnObjectFeatureStruct( const std::vector< NetnObjectDefinitionStruct >& subObjectList );
    explicit NetnObjectFeatureStruct( const NetnObjectDescription& objectDetail );
    explicit NetnObjectFeatureStruct( const NetnHumanDescription& humanDetail );
    explicit NetnObjectFeatureStruct( const NetnEquipDescription& equipDetail );
    explicit NetnObjectFeatureStruct( const NetnPlateformDescription& plateformDetail );
    virtual ~NetnObjectFeatureStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << featureLevel;
        if( featureLevel == 1 )
            archive << objectDetail;
        if( featureLevel == 2 )
            archive << humanDetail;
        if( featureLevel == 3 )
            archive << equipDetail;
        if( featureLevel == 4 )
            archive << plateformDetail;
        if( featureLevel == 5 )
        {
            uint32 size = static_cast< uint32 >( subObjectList.size() );
            archive << size
                    << subObjectList;
        }
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> featureLevel;
        if( featureLevel == 1 )
            archive >> objectDetail;
        if( featureLevel == 2 )
            archive >> humanDetail;
        if( featureLevel == 3 )
            archive >> equipDetail;
        if( featureLevel == 4 )
            archive >> plateformDetail;
        if( featureLevel == 5 )
        {
            uint32 size = 0;
            archive >> size;
            subObjectList.resize( size );
            archive >> subObjectList;
        }
    }
    //@}

    //! @name Member data
    //@{
    int32 featureLevel;
    std::vector< NetnObjectDefinitionStruct > subObjectList;
    NetnObjectDescription objectDetail;
    NetnHumanDescription humanDetail;
    NetnEquipDescription equipDetail;
    NetnPlateformDescription plateformDetail;
    //@}
};

// =============================================================================
/** @class  NetnObjectDefinitionStruct
    @brief  Netn object definition struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnObjectDefinitionStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnObjectDefinitionStruct();
             NetnObjectDefinitionStruct( const std::string& callsign, const std::vector< char >& uniqueId,
                                         const NetnObjectFeatureStruct& objectFeature );
    virtual ~NetnObjectDefinitionStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint8 padding = 0;
        archive << callsign
                << uniqueId << padding;
        if( callsign.str().size() % 2 )
            archive << padding << padding;
        archive << objectFeature;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint8 padding = 0;
        archive >> callsign
                >> uniqueId >> padding;
        if( callsign.str().size() % 2 )
            archive >> padding >> padding;
        archive >> objectFeature;
    }
    //@}

public:
    //! @name Member data
    //@{
    UnicodeString callsign;
    std::vector< char > uniqueId;
    NetnObjectFeatureStruct objectFeature;
    //@}
};

// =============================================================================
/** @class  NetnAppointmentStruct
    @brief  Netn appointment struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnAppointmentStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnAppointmentStruct();
             NetnAppointmentStruct( int64 dateTime, const rpr::WorldLocation& location );
             NetnAppointmentStruct( const NetnAppointmentStruct& other );
    virtual ~NetnAppointmentStruct();
    //@}

    //! @name Operations
    //@{
    const NetnAppointmentStruct& operator=( const NetnAppointmentStruct& rhs );
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << dateTime
                << location;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> dateTime
                >> location;
    }
    //@}

public:
    //! @name Member data
    //@{
    int64 dateTime;
    rpr::WorldLocation location;
    //@}
};

// =============================================================================
/** @class  NetnDataTStruct
    @brief  Netn data t struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnDataTStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnDataTStruct();
             NetnDataTStruct( const std::vector< NetnObjectDefinitionStruct >& objectToManage,
                              const NetnAppointmentStruct& appointment,
                              const NetnAppointmentStruct& finalAppointment );
    virtual ~NetnDataTStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint32 size = static_cast< uint32 >( objectToManage.size() );
        archive << size
                << objectToManage;
        const unsigned int padding = archive.GetSize() % 8;
        for( unsigned int i = 0; i < padding; ++i )
            archive << static_cast< int8 >( 0 );
        archive << appointment
                << finalAppointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        const unsigned int start = archive.GetSize();
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        if( size != 0 )
            archive >> objectToManage;
        const unsigned int padding = ( archive.GetSize() - start ) % 8;
        int8 junk;
        for( unsigned int i = 0; i < padding; ++i )
            archive >> junk;
        archive >> appointment
                >> finalAppointment;
    }
    //@}

public:
    //! @name Member data
    //@{
    std::vector< NetnObjectDefinitionStruct > objectToManage;
    NetnAppointmentStruct appointment;
    NetnAppointmentStruct finalAppointment;
    //@}
};

// =============================================================================
/** @class  NetnDataEDStruct
    @brief  Netn data ed struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnDataEDStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnDataEDStruct();
             NetnDataEDStruct( const std::vector< NetnObjectDefinitionStruct >& objectToManage,
                               const NetnAppointmentStruct& appointment );
    virtual ~NetnDataEDStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        // TODO AHC padding
        const uint32 size = static_cast< uint32 >( objectToManage.size() );
        archive << size
                << objectToManage;

        const unsigned int padding = archive.GetSize() % 8;
        for( unsigned int i = 0; i < padding; ++i )
            archive << static_cast< int8 >( 0 );

        archive << appointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        const unsigned int start = archive.GetSize();
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        if( size != 0 )
            archive >> objectToManage;

        const unsigned int padding = ( archive.GetSize() - start ) % 8;
        int8 junk;
        for( unsigned int i = 0; i < padding; ++i )
            archive >> junk;

        archive >> appointment;
    }
    //@}

public:
    //! @name Member data
    //@{
    std::vector< NetnObjectDefinitionStruct > objectToManage;
    NetnAppointmentStruct appointment;
    //@}
};

// =============================================================================
/** @class  NetnEventIdentifier
    @brief  Netn event identifier
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnEventIdentifier
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnEventIdentifier();
             NetnEventIdentifier( int32 eventCount, const std::string& issuingObjectIdentifier );
    virtual ~NetnEventIdentifier();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << eventCount
                << issuingObjectIdentifier;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> eventCount
                >> issuingObjectIdentifier;
    }
    //@}

public:
    //! @name Member data
    //@{
    int32 eventCount;
    UnicodeString issuingObjectIdentifier;
    //@}
};

// =============================================================================
/** @class  NetnTransportStruct
    @brief  Netn transport struct
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnTransportStruct
{
public:
    enum ConvoyType
    {
        E_Transport     = 0,
        E_Embarkment    = 1,
        E_Disembarkment = 2
    };

    //! @name Constructors/Destructor
    //@{
             NetnTransportStruct();
    explicit NetnTransportStruct( const NetnDataTStruct& dataTransport );
             NetnTransportStruct( const NetnDataEDStruct& data, ConvoyType convoyType );
    virtual ~NetnTransportStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint32 padding = 0;
        archive << static_cast<int32>(convoyType) << padding;
        if( convoyType == E_Transport )
            archive << dataTransport;
        if( convoyType == E_Embarkment )
            archive << dataEmbarkment;
        if( convoyType == E_Disembarkment )
            archive << dataDisembarkment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32 padding = 0;
        int32 convoy;
        archive >> convoy >> padding;
        convoyType = static_cast<ConvoyType>( convoy );
        if( convoyType == E_Transport )
            archive >> dataTransport;
        if( convoyType == E_Embarkment )
            archive >> dataEmbarkment;
        if( convoyType == E_Disembarkment )
            archive >> dataDisembarkment;
    }
    //@}

public:
    //! @name Member data
    //@{
    ConvoyType convoyType;
    NetnDataTStruct dataTransport;
    NetnDataEDStruct dataEmbarkment;
    NetnDataEDStruct dataDisembarkment;
    //@}
};

// NETNv2

struct Netn2DataTStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             Netn2DataTStruct();
             Netn2DataTStruct( const std::vector< NETN_UUID >& objectToManage,
                              const NetnAppointmentStruct& appointment,
                              const NetnAppointmentStruct& finalAppointment );
    virtual ~Netn2DataTStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint32 size = static_cast< uint32 >( objectToManage.size() );
        archive << size
                << objectToManage;
        const unsigned int padding = archive.GetSize() % 8;
        for( unsigned int i = 0; i < padding; ++i )
            archive << static_cast< int8 >( 0 );
        archive << appointment
                << finalAppointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        const unsigned int start = archive.GetSize();
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        archive >> objectToManage;
        const unsigned int padding = ( archive.GetSize() - start ) % 8;
        int8 junk;
        for( unsigned int i = 0; i < padding; ++i )
            archive >> junk;
        archive >> appointment
                >> finalAppointment;
    }
    //@}
public:
    //! @name Member data
    //@{
    std::vector< NETN_UUID > objectToManage;
    NetnAppointmentStruct appointment;
    NetnAppointmentStruct finalAppointment;
    //@}
};

struct Netn2DataEDStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             Netn2DataEDStruct();
             Netn2DataEDStruct( const std::vector< NETN_UUID >& objectToManage,
                               const NetnAppointmentStruct& appointment );
    virtual ~Netn2DataEDStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        // TODO AHC padding
        const uint32 size = static_cast< uint32 >( objectToManage.size() );
        archive << size
                << objectToManage;

        const unsigned int padding = archive.GetSize() % 8;
        for( unsigned int i = 0; i < padding; ++i )
            archive << static_cast< int8 >( 0 );

        archive << appointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        const unsigned int start = archive.GetSize();
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        archive >> objectToManage;

        const unsigned int padding = ( archive.GetSize() - start ) % 8;
        int8 junk;
        for( unsigned int i = 0; i < padding; ++i )
            archive >> junk;

        archive >> appointment;
    }
    //@}

public:
    //! @name Member data
    //@{
    std::vector< NETN_UUID > objectToManage;
    NetnAppointmentStruct appointment;
    //@}
};

struct Netn2TransportStruct
{
public:    
    enum TransportType
    {
        E_Transport     = 0,
        E_Embarkment    = 1,
        E_Disembarkment = 2
    };

    //! @name Constructors/Destructor
    //@{
             Netn2TransportStruct();
    explicit Netn2TransportStruct( const Netn2DataTStruct& dataTransport );
             Netn2TransportStruct( const Netn2DataEDStruct& data, TransportType transportType );
    virtual ~Netn2TransportStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint32 padding = 0;
        archive << static_cast<int32>( transportType ) << padding;
        if( transportType == E_Transport )
            archive << dataTransport;
        if( transportType == E_Embarkment )
            archive << dataEmbarkment;
        if( transportType == E_Disembarkment )
            archive << dataDisembarkment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32 padding = 0;
        int32 convoy;
        archive >> convoy >> padding;
        transportType = static_cast<TransportType>( convoy );
        if( transportType == E_Transport )
            archive >> dataTransport;
        if( transportType == E_Embarkment )
            archive >> dataEmbarkment;
        if( transportType == E_Disembarkment )
            archive >> dataDisembarkment;
    }
    //@}
public:
    //! @name Member data
    //@{
    TransportType transportType;
    Netn2DataTStruct dataTransport;
    Netn2DataEDStruct dataEmbarkment;
    Netn2DataEDStruct dataDisembarkment;
    //@}
};

}
}

#endif // plugins_hla_Transportation_h
