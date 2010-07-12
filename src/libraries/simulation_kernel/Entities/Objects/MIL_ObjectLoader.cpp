// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectLoader.h"
#include "AttributeFactory.h"
#include "CapacityFactory.h"
#include "Object.h"
#include "ObjectPrototype.h"
#include "UrbanObjectWrapper.h"
#include "Entities/MIL_Army_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    MIL_ObjectLoader* pLoader_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::GetLoader
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
MIL_ObjectLoader& MIL_ObjectLoader::GetLoader()
{
    if( !pLoader_ )
        pLoader_ = new MIL_ObjectLoader();
    return *pLoader_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader constructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectLoader::MIL_ObjectLoader()
    : factory_( new CapacityFactory() )
    , attributes_( new AttributeFactory() )
{
    if( !pLoader_ )
        pLoader_ = this;
    else
        throw std::runtime_error( "MIL_ObjectLoader already created" );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectLoader::~MIL_ObjectLoader()
{
    pLoader_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::GetType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectLoader::GetType( const std::string& type ) const
{
    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw std::runtime_error( __FUNCTION__ " - Unknown object type: " + type );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::GetCapactityFactory
// Created: JSR 2010-03-12
// -----------------------------------------------------------------------------
const CapacityFactory& MIL_ObjectLoader::GetCapacityFactory() const
{
    return *factory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::Initialize
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
            >> xml::list( "object", *this, &MIL_ObjectLoader::ReadObjectPrototype )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::ReadObjectPrototype( xml::xistream& xis )
{
    const std::string type( xml::attribute< std::string >( xis, "type" ) );
    boost::shared_ptr< ObjectPrototype >& prototype = prototypes_[ type ];
    if( prototype.get() )
        throw std::runtime_error( __FUNCTION__ " - Object type redefinition: " + type );
    prototype.reset( new ObjectPrototype( type, prototypes_.size() ) );
    xis >> xml::list( *this, &MIL_ObjectLoader::ReadCapacity, *prototype );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadCapacity
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::ReadCapacity( const std::string& capacity, xml::xistream& xis, ObjectPrototype& prototype )
{
    factory_->Create( prototype, capacity, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& location, bool reserved ) const
{
    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw std::runtime_error( __FUNCTION__ " - Unknown object type: " + type );
    Object* object = new Object( *it->second, army, &location, "", reserved );
    object->Finalize();
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( xml::xistream& xis, MIL_Army_ABC& army ) const
{
    const std::string type( xml::attribute< std::string >( xis, "type" ) );

    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw std::runtime_error( __FUNCTION__ " - Unknown object prototype: " + type );
    TER_Localisation location;
    location.Read( xis );
    // $$$$ SBO 2009-06-08: Check geometry constraint
    Object* object = new Object( xis, *it->second, army, &location );
    xis >> xml::optional >> xml::start( "attributes" )
            >> xml::list( *this, &MIL_ObjectLoader::ReadAttributes, *object )
        >> xml::end;
    object->Finalize();
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( const Common::MsgMissionParameters& message, MIL_Army_ABC& army, MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode& value ) const
{
    CIT_Prototypes it = prototypes_.find( message.elem( 0 ).value().acharstr() );
    if( it == prototypes_.end() )
    {
        value = MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_object;
        return 0;
    }
    TER_Localisation location;
    if( ! NET_ASN_Tools::ReadLocation( message.elem( 1 ).value().location(), location ) )
        return 0;
    Object* pObject = new Object( *it->second, army, &location, message.elem( 2 ).value().acharstr() );
    attributes_->Create( *pObject, message.elem( 4 ).value() );
    pObject->Finalize();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army ) const
{
    CIT_Prototypes it = prototypes_.find( builder.GetType().GetName() );
    if( it == prototypes_.end() )
        return 0;
    Object* pObject = new Object( *it->second, army, 0 );
    builder.Build( *pObject );
    pObject->Finalize();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateUrbanObject
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateUrbanObject( const urban::TerrainObject_ABC& object ) const
{
    CIT_Prototypes it = prototypes_.find( "urban block" );
    if( it == prototypes_.end() )
        return 0;
    UrbanObjectWrapper* pObject = new UrbanObjectWrapper( *it->second, object );
    // pObject->Finalize();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadAttributes
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::ReadAttributes( const std::string& attribute, xml::xistream& xis, Object& object ) const
{
    attributes_->Create( object, attribute, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::InitializeLocation
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode MIL_ObjectLoader::InitializeLocation( Object& object, const Common::MsgLocation& asn ) const
{
    TER_Localisation location;
    if( ! NET_ASN_Tools::ReadLocation( asn, location ) )
        return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes;
    object.Initialize( location );
    return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;
}