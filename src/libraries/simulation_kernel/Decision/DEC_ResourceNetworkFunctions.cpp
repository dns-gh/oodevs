// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ResourceNetworkFunctions.h"
#include "DEC_ResourceNetwork.h"
#include "MIL_AgentServer.h"
#include "PHY_ResourceNetworkType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_ResourceNetworkFunctions::GetResourceNetworkPosition
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_ResourceNetworkFunctions::GetResourceNetworkPosition( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    if( resourceNetwork )
        if( MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( resourceNetwork->GetObjectId() ) )
            return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( object->GetLocalisation().ComputeBarycenter() ) );
    return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DeactivateRessourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DeactivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    DoActivateRessourceNetworkElement( resourceNetwork, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::ActivateRessourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::ActivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    DoActivateRessourceNetworkElement( resourceNetwork, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DoActivateRessourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DoActivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation )
{
    if( !resourceNetwork )
        return;
    const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() );
    if( !type )
        return;
    MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( resourceNetwork->GetObjectId() );
    if( !object )
        return;
    ResourceNetworkCapacity* capacity = object->Retrieve< ResourceNetworkCapacity >();
    if( capacity )
        capacity->SetActivation( type->GetId(), activation );
}
