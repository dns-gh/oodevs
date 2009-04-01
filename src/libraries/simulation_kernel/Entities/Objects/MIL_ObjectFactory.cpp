// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectFactory.h"
#include "MIL_ObjectLoader.h"
#include "MIL_ObjectManager.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectManipulator_ABC.h"

using namespace xml;

MIL_ObjectLoader* MIL_ObjectFactory::pLoader_ = 0;

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory constructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::MIL_ObjectFactory( MIL_ObjectManager& manager )
    : manager_ ( manager )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::~MIL_ObjectFactory()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::GetLoader
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
MIL_ObjectLoader& MIL_ObjectFactory::GetLoader()
{
    if ( !pLoader_ )
        pLoader_ = new MIL_ObjectLoader();
    return *pLoader_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::Initialize
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Initialize( xml::xistream& xis )
{
    GetLoader().Initialize( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::FindType
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectFactory::FindType( const std::string& type )
{    
    return GetLoader().GetType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::BuildObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    MIL_Object_ABC* pObject = GetLoader().CreateObject( xis, army );
    if ( pObject )
        manager_.RegisterObject( *pObject );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ObjectFactory::BuildObject( const ASN1T_MagicActionCreateObject& asn, MIL_Army_ABC& army )
{
    ASN1T_EnumObjectErrorCode   value = EnumObjectErrorCode::no_error;
    MIL_Object_ABC*             pObject = GetLoader().CreateObject( asn, army, value );
    if ( pObject )
    {
        MIL_ObjectManipulator_ABC& obj = pObject->operator ()();
        obj.Construct();
        manager_.RegisterObject( *pObject );
    }
    return value;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, ASN1T_EnumDemolitionTargetType obstacleType )
{
    MIL_Object_ABC* pObject = GetLoader().CreateObject( type, army, localisation, obstacleType );
    if ( pObject )
        manager_.RegisterObject( *pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army ) 
{
    MIL_Object_ABC* pObject = GetLoader().CreateObject( builder, army );
    if ( pObject )
        manager_.RegisterObject( *pObject );
    return pObject;
}
