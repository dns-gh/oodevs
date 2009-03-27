// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectLoader.h"
#include "ObjectPrototype.h"
#include "CapacityFactory.h"
#include "AttributeFactory.h"
#include "Object.h"
//#include "MIL_AgentServer.h"
#include "Entities/MIL_Army_ABC.h"
//#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_IDManager.h"

#include <xeumeuleu/xml.h>
#include <boost/shared_ptr.hpp>

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader constructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectLoader::MIL_ObjectLoader()
    : factory_      ( new CapacityFactory() )
    , attributes_   ( new AttributeFactory() )
    , id_           ( MIL_IDManager::objects_ )
{
    //NOTHING
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectLoader::~MIL_ObjectLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::GetType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectLoader::GetType( const std::string& type ) const
{
    const CIT_Prototypes cit = prototypes_.find( type );
    if ( cit != prototypes_.end() )
        return *(cit->second);
    throw std::runtime_error( "Unknown object type - " + type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::Initialize
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::Initialize( xml::xistream& xis )
{
    xis >> start( "objects" )
            >> list( "object", *this, &MIL_ObjectLoader::ReadObjectPrototype )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
void MIL_ObjectLoader::ReadObjectPrototype( xistream& xis )
{    
    std::string type( attribute( xis, "type", std::string() ) );
    if ( prototypes_.find( type ) == prototypes_.end() )
    {
        ObjectPrototype* prototype = new ObjectPrototype( type, prototypes_.size() );
        prototypes_[ type ].reset( prototype ); 
        xis >> xml::list( *this, &MIL_ObjectLoader::ReadCapacity, *prototype );
    }
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
    if ( it != prototypes_.end() )
    {        
        Object* pObject = new Object( id_.GetFreeSimID(), *it->second, army, &location, std::string(), reserved );
        pObject->Finalize();
        return pObject;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( xistream& xis, MIL_Army_ABC& army ) const
{    
    const int id = attribute( xis, "id", 0 );
    std::string type( attribute( xis, "type", std::string() ) );
    std::string name( attribute( xis, "name", std::string() ) );
    
    CIT_Prototypes it = prototypes_.find( type );
    if ( it != prototypes_.end() )
    {
        TER_Localisation location;
        location.Read( xis );
        Object* pObject = new Object( id, *it->second, army, &location, name );
        xis >> xml::optional() 
            >> xml::start( "attributes" )
                >> xml::list( *this, &MIL_ObjectLoader::ReadAttributes, *pObject )
            >> end();
        pObject->Finalize();
        return pObject;
    }
    throw std::runtime_error( "Unknown prototype : " + type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( const ASN1T_MagicActionCreateObject& asn, MIL_Army_ABC& army, ASN1T_EnumObjectErrorCode& value ) const
{
    CIT_Prototypes it = prototypes_.find( asn.type ); // asn.type
    if ( it != prototypes_.end() )
    {
        TER_Localisation location;
        if( ! NET_ASN_Tools::ReadLocation( asn.location, location ) )
            return 0;
        Object* pObject = new Object( id_.GetFreeSimID(), *it->second, army, &location, asn.name );
        attributes_->Create( *pObject, asn.attributes );
        pObject->Finalize();
        return pObject;        
    }
    else
        value = EnumObjectErrorCode::error_invalid_object;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectLoader::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectLoader::CreateObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army ) const
{
    CIT_Prototypes it = prototypes_.find( builder.GetType().GetName() );
    if ( it != prototypes_.end() )
    {        
        Object* pObject = new Object( id_.GetFreeSimID(), *it->second, army, 0 );
		builder.Build( *pObject );
        pObject->Finalize();
        return pObject;
    }
    return 0;
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
ASN1T_EnumObjectErrorCode MIL_ObjectLoader::InitializeLocation( Object& object, const ASN1T_Location& asn ) const
{
    TER_Localisation location;
    if( ! NET_ASN_Tools::ReadLocation( asn, location ) )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    object.Initialize( location );
    return EnumObjectErrorCode::no_error;
}