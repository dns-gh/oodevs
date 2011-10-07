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
#include <hla/HLA_Types.h>

namespace plugins
{
namespace hla
{

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
    virtual ~NetnObjectFeatureStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << featureLevel;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> featureLevel;
    }
    //@}

    //! @name Member data
    //@{
    int32 featureLevel;
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
             NetnObjectDefinitionStruct( const std::string& callsign, const std::string& uniqueId );
    virtual ~NetnObjectDefinitionStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << callsign
                << uniqueId
                << objectFeature;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> callsign
                >> uniqueId
                >> objectFeature;
    }
    //@}

public:
    //! @name Member data
    //@{
    UnicodeString callsign;
    UniqueId uniqueId;
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
    virtual ~NetnAppointmentStruct();
    //@}

    //! @name Operations
    //@{
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
        const uint32 size = objectToManage.size();
        archive << size
                << objectToManage
                << appointment
                << finalAppointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        archive >> objectToManage
                >> appointment
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
        const uint32 size = objectToManage.size();
        archive << size
                << objectToManage
                << appointment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32 size = 0;
        archive >> size;
        objectToManage.resize( size );
        archive >> objectToManage
                >> appointment;
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
    //! @name Constructors/Destructor
    //@{
             NetnTransportStruct();
    explicit NetnTransportStruct( const NetnDataTStruct& dataTransport );
             NetnTransportStruct( const NetnDataEDStruct& data, int32 convoyType );
    virtual ~NetnTransportStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << convoyType;
        if( convoyType == 0 )
            archive << dataTransport;
        if( convoyType == 1 )
            archive << dataEmbarkment;
        if( convoyType == 2 )
            archive << dataDisembarkment;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> convoyType;
        if( convoyType == 0 )
            archive >> dataTransport;
        if( convoyType == 1 )
            archive >> dataEmbarkment;
        if( convoyType == 2 )
            archive >> dataDisembarkment;
    }
    //@}

public:
    //! @name Member data
    //@{
    int32 convoyType;
    NetnDataTStruct dataTransport;
    NetnDataEDStruct dataEmbarkment;
    NetnDataEDStruct dataDisembarkment;
    //@}
};
}
}

#endif // plugins_hla_Transportation_h
