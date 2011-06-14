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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"

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
// Name: DEC_ResourceNetworkFunctions::DeactivateResourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DeactivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    DoActivateResourceNetworkElement( resourceNetwork, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::ActivateResourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::ActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    DoActivateResourceNetworkElement( resourceNetwork, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DoActivateResourceNetworkElement
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DoActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation )
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

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkLink
// Created: GGE 2011-06-10
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::CreateResourceNetworkLink( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production )
{
    if( !caller || !target || !pLocalisation )
        return;
    const PHY_ResourceNetworkType* resourceType = PHY_ResourceNetworkType::Find( target->GetResource() );
    if( !resourceType )
        return;
    MIL_Object_ABC* objectTarget = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( target->GetObjectId() );
    if( !objectTarget || !objectTarget->Retrieve< ResourceNetworkCapacity >())
        return;
    MIL_Object_ABC* objectSource = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( type, caller->GetPion().GetArmy(), *pLocalisation );
    if( ResourceNetworkCapacity* capacitySource = objectSource->Retrieve< ResourceNetworkCapacity >() )
        capacitySource->CreateLink( objectTarget->GetID(), resourceType->GetId(), production );
}
