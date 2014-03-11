// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FOM_Serializer.h"
#include "Spatial.h"
#include "ObjectListener_ABC.h"
#include "UnicodeString.h"
#include "SilentEntity.h"
#include "IsPartOfStruct.h"
#include "EntityIdentifierResolver_ABC.h"
#include "SerializationTools.h"

#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityAppearance.h"

#include <hla/Deserializer_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FOM_Serializer constructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
FOM_Serializer::FOM_Serializer(  int netnVersion )
    : netnVersion_ ( netnVersion )
    , uniqueIdSerializer_( new UniqueIdSerializer( netnVersion ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer destructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
FOM_Serializer::~FOM_Serializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadSpatial
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Spatial& spatial )
{
    spatial.Deserialize( deserializer );
    listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadForceIdentifier
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::ForceIdentifier& force )
{
    int8_t tmpForce;
    deserializer >> tmpForce;
    listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( tmpForce ) );
    force = static_cast< rpr::ForceIdentifier >( tmpForce );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadAggregateMarking
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadAggregateMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, AggregateMarking& marking )
{
    marking.Deserialize( deserializer );
    listener.NameChanged( identifier, marking.str() );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadEntityType
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type )
{
    type.Deserialize( deserializer );
    listener.TypeChanged( identifier, type );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadNumberOfSilentEntities
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadNumberOfSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, unsigned short& numberOfSilentEntities )
{
    deserializer >> numberOfSilentEntities;
}

#pragma warning( push )
#pragma warning( disable : 4700 ) // uninitialized variable for union
// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadSilentEntities
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadSilentEntities( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, unsigned int numberOfSilentEntities )
{
    for( unsigned int i = 0; i < numberOfSilentEntities; ++i )
    {
        SilentEntity entity;
        entity.Deserialize( deserializer );
        unsigned int available = entity.numberOfEntitiesOfThisType_, dead = 0, lightDamages = 0, heavyDamages = 0;
        for( auto it = entity.entityAppearance_.begin(); it != entity.entityAppearance_.end(); ++it )
        {
            rpr::EntityAppearance_Land app;
            app.value_ = *it;
            switch( app.Damage() )
            {
            case 0:
                break;
            case 1:
                available--;
                lightDamages++;
                break;
            case 2:
                available--;
                heavyDamages++;
                break;
            case 3:
                available--;
                dead++;
                break;
            }
        }
        listener.EquipmentUpdated( identifier, entity.entityType_, available, dead, lightDamages, heavyDamages );
    }
}
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadEntityIdentifier
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadEntityIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& /*listener*/, rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver )
{
    entityId.Deserialize( deserializer );
    entityIdentifierResolver.Unregister( identifier );
    entityIdentifierResolver.Register( entityId, identifier );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadNothing
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadNothing( ::hla::Deserializer_ABC& /*deserializer*/, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadUnsignedChar
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadUnsignedChar( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, unsigned char& value)
{
    deserializer >> value;
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadCallsign
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadCallsign( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& callsign )
{
    callsign.Deserialize( deserializer );
    listener.CallsignChanged( identifier, callsign.str() );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadUniqueId
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadUniqueId( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< char >& uniqueId )
{
    switch( netnVersion_ )
    {
    case 1:
        {
            UniqueId u;
            u.Deserialize( deserializer );
            u.Read( uniqueId );            
            listener.UniqueIdChanged( identifier, u.data() );
        }
        break;
    case 2:
        {
            NETN_UUID u;
            u.Deserialize( deserializer );
            u.Read( uniqueId );
            listener.UniqueIdChanged( identifier, u.data() );
        } 
        break;
    default:
        throw MASA_EXCEPTION("wrong NETN version") ;
    }
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadMounted
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadMounted( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, double& mounted )
{
    deserializer >> mounted;
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadSymbol
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadSymbol( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, UnicodeString& symbol )
{
    symbol.Deserialize( deserializer );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadStatus
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadStatus( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, int8_t& status )
{
    deserializer >> status;
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadEmbeddedUnitList
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadEmbeddedUnitList( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
{
    std::vector< ObjectListener_ABC::T_UniqueId > embeddedUnits;
    uint32_t size;
    deserializer >> size;
    embeddedUnits.resize(size);
    for(uint32_t i=0; i < size; ++i )
    {
        switch( netnVersion_ )
        {
        case 1:
            {
                UniqueId tmp;
                tmp.Deserialize( deserializer );
                embeddedUnits[i]=tmp.data();
            }
            break;
        case 2:
            {
                NETN_UUID tmp;
                tmp.Deserialize( deserializer );
                embeddedUnits[i]=tmp.data();
            }
            break;
         }
    }
    listener.EmbeddedUnitListChanged( identifier, embeddedUnits );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadMarking
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Marking& marking )
{
    marking.Deserialize( deserializer );
    listener.NameChanged( identifier, marking.str() );
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadMarking
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
const UniqueIdSerializer& FOM_Serializer::GetUniqueIdSerializer()
{
    return *uniqueIdSerializer_;
}

// -----------------------------------------------------------------------------
// Name: FOM_Serializer::ReadIsPartOf
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void FOM_Serializer::ReadIsPartOf( ::hla::Deserializer_ABC& /*deserializer*/, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, IsPartOfStruct& /*isPartOf*/ )
{
    // NOTHING
}
