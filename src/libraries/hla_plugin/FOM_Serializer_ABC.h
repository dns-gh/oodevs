// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FOM_Serializer_ABC_H
#define plugins_hla_FOM_Serializer_ABC_H

#include <string>
#include <hla/HLA_Types.h>

#include "rpr/ForceIdentifier.h"
#include "rpr/Enums.h"
#include "UniqueId.h"
#include "AggregateMarking.h"

namespace hla
{
    class Deserializer_ABC;
    class Serializer_ABC;
}

namespace rpr
{
    class EntityIdentifier;
    class EntityType;
}

namespace plugins
{
namespace hla
{

class ObjectListener_ABC;
class Spatial;
class EntityIdentifierResolver_ABC;
class UnicodeString;
struct uniqueIdSerializer;
class IsPartOfStruct;

// =============================================================================
/** @class  FOM_Serializer_ABC
    @brief  FOM_Serializer_ABC
*/
// Created: AHC 2012-09-03
// =============================================================================
class FOM_Serializer_ABC
{
public:
    virtual ~FOM_Serializer_ABC() {}

    //! @name Operations
    //@{
    virtual void ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Spatial& spatial ) = 0;
    virtual void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::ForceIdentifier& force ) = 0;
    virtual void ReadAggregateMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, AggregateMarking& marking ) = 0;
    virtual void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type ) = 0;
    virtual void ReadNumberOfSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned short& numberOfSilentEntities ) = 0;
    virtual void ReadSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned int numberOfSilentEntities ) = 0;
    virtual void ReadEntityIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver ) = 0;
    virtual void ReadNothing( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener ) = 0;
    virtual void ReadUnsignedChar( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned char& value) = 0;
    virtual void ReadMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Marking& marking ) = 0;
    virtual void ReadIsPartOf( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, IsPartOfStruct& isPartOf ) = 0;

    virtual void ReadCallsign( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& callsign ) = 0;
    virtual void ReadUniqueId( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< char >& uniqueId ) = 0;
    virtual void ReadMounted( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, double& mounted ) = 0;
    virtual void ReadSymbol( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& symbol ) = 0;
    virtual void ReadStatus( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, int8_t& status ) = 0;
    virtual void ReadEmbeddedUnitList( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener ) = 0;

    virtual const UniqueIdSerializer& GetUniqueIdSerializer() = 0;


    //@}
};

}
}
#endif // plugins_hla_FOM_Serializer_ABC_H
