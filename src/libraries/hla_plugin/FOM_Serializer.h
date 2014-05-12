// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FOM_Serializer_H_
#define plugins_hla_FOM_Serializer_H_

#include "FOM_Serializer_ABC.h"

namespace plugins
{
namespace hla
{

/// =============================================================================
/// @class hla::FOM_Serializer
/// @brief hla::FOM_Serializer
/// @thread This type is not thread safe
///
/// Created: ahc 3 sept. 2012
/// =============================================================================
class FOM_Serializer : public FOM_Serializer_ABC
{
public:
    FOM_Serializer( int netnVersion );
    virtual ~FOM_Serializer();

    //! @name Operations
    //@{
    virtual void ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Spatial& spatial );
    virtual void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::ForceIdentifier& force );
    virtual void ReadAggregateMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, AggregateMarking& marking );
    virtual void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type );
    virtual void ReadNumberOfSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned short& numberOfSilentEntities );
    virtual void ReadSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned int numberOfSilentEntities );
    virtual void ReadEntityIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver );
    virtual void ReadNothing( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener );
    virtual void ReadUnsignedChar( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned char& value);
    virtual void ReadMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Marking& marking );
    virtual void ReadIsPartOf( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, IsPartOfStruct& isPartOf );

    virtual void ReadCallsign( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& callsign );
    virtual void ReadUniqueId( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< char >& uniqueId );
    virtual void ReadMounted( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, double& mounted );
    virtual void ReadSymbol( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& symbol );
    virtual void ReadStatus( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, int8_t& status );
    virtual void ReadEmbeddedUnitList( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener );

    virtual const UniqueIdSerializer& GetUniqueIdSerializer() ;
    //@}

private:
    int netnVersion_;
    std::unique_ptr< UniqueIdSerializer > uniqueIdSerializer_;
};

}
}
#endif // plugins_hla_FOM_Serializer_H_
