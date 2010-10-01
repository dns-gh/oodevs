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
#include "protocol/protocol.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "MT_Tools/MT_ScipioException.h"

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
// Name: MIL_ObjectFactory::Initialize
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Initialize( xml::xistream& xis )
{
    MIL_ObjectLoader::GetLoader().Initialize( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::FindType
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectFactory::FindType( const std::string& type )
{
    return MIL_ObjectLoader::GetLoader().GetType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
MIL_Object_ABC& MIL_ObjectFactory::BuildObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    MIL_Object_ABC* pObject = MIL_ObjectLoader::GetLoader().CreateObject( xis, army );
    if( pObject )
    {
        manager_.RegisterObject( *pObject );
        return *pObject;
    }
    else
    {
        //@TODO MGD propaguade reference
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode MIL_ObjectFactory::BuildObject( const Common::MsgMissionParameters& msg, MIL_Army_ABC& army )
{
    MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode value = MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;
    MIL_Object_ABC* pObject =  MIL_ObjectLoader::GetLoader().CreateObject( msg, army, value );
    if( pObject )
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
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, Common::ObstacleType_DemolitionTargetType obstacleType )
{
    MIL_Object_ABC* pObject = MIL_ObjectLoader::GetLoader().CreateObject( type, army, localisation, obstacleType == Common::ObstacleType_DemolitionTargetType_reserved );
    if( pObject )
        manager_.RegisterObject( *pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army )
{
    MIL_Object_ABC* pObject = MIL_ObjectLoader::GetLoader().CreateObject( builder, army );
    if( pObject )
        manager_.RegisterObject( *pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildUrbanObject
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildUrbanObject( const urban::TerrainObject_ABC& object )
{
    MIL_Object_ABC* pObject = MIL_ObjectLoader::GetLoader().CreateUrbanObject( object );
    if( pObject )
        manager_.RegisterObject( *pObject );
    return pObject;
}
