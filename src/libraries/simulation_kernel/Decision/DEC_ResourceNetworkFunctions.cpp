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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Urban/PHY_ResourceNetworkType.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include <boost/make_shared.hpp>

namespace
{
    ResourceNetworkCapacity* GetCapacity( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
    {
        if( resourceNetwork )
            if( MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( resourceNetwork->GetObjectId() ) )
                return object->Retrieve< ResourceNetworkCapacity >();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_ResourceNetworkFunctions::GetResourceNetworkPosition
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_ResourceNetworkFunctions::GetResourceNetworkPosition( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    if( resourceNetwork )
        if( MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( resourceNetwork->GetObjectId() ) )
            return boost::make_shared< MT_Vector2D >( object->GetLocalisation().ComputeBarycenter() );
    return boost::make_shared< MT_Vector2D >();
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
    if( ResourceNetworkCapacity* capacity = GetCapacity( resourceNetwork ) )
        if( const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() ) )
            capacity->SetActivation( type->GetId(), activation );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkObjectAndLink
// Created: GGE 2011-06-10
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::CreateResourceNetworkObjectAndLink( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production )
{
    CreateResourceNetworkObjectAndLinkReturn( caller, type, pLocalisation, target, production );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkObjectAndLinkReturn
// Created: GGE 2011-06-10
// -----------------------------------------------------------------------------
unsigned int DEC_ResourceNetworkFunctions::CreateResourceNetworkObjectAndLinkReturn( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production )
{
    if( !caller || !target || !pLocalisation )
        return 0;
    const PHY_ResourceNetworkType* resourceType = PHY_ResourceNetworkType::Find( target->GetResource() );
    if( !resourceType )
        return 0;
    MIL_Object_ABC* objectTarget = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( target->GetObjectId() );
    if( !objectTarget || !objectTarget->Retrieve< ResourceNetworkCapacity >() )
        return 0;
    MIL_Object_ABC* objectSource = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( type, &caller->GetPion().GetArmy(), *pLocalisation );
    if( !objectSource )
        return 0;
    if( ResourceNetworkCapacity* capacitySource = objectSource->Retrieve< ResourceNetworkCapacity >() )
        capacitySource->CreateLink( objectTarget->GetID(), resourceType->GetId(), production );
    return objectSource->GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DestroyResourceNetworkObjectLink
// Created: GGE 2011-06-27
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DestroyResourceNetworkObjectLink( unsigned int objectResourceID )
{
    MIL_Object_ABC* objectToDestroy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectResourceID );
    if( objectToDestroy && ( *objectToDestroy )().CanBeDestroyed() )
        ( *objectToDestroy )().Destroy();
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::IncreaseResourceProduction
// Created: GGE 2011-08-02
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::IncreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production )
{
    if( ResourceNetworkCapacity* capacity = GetCapacity( resourceNetwork ) )
        if( const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() ) )
            capacity->AddProduction( type->GetId(), production );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DecreaseResourceProduction
// Created: GGE 2011-08-02
// -----------------------------------------------------------------------------
void DEC_ResourceNetworkFunctions::DecreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production )
{
    if( ResourceNetworkCapacity* capacity = GetCapacity( resourceNetwork ) )
        if( const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() ) )
            capacity->DecreaseProduction( type->GetId(), production );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::GetResourceNetworksInZone
// Created: JSR 2012-01-17
// -----------------------------------------------------------------------------
T_ResourceNetworkVector DEC_ResourceNetworkFunctions::GetResourceNetworksInZone( DEC_Decision_ABC* caller, const TER_Localisation* pLocalisation )
{
    T_ResourceNetworkVector result;
    if( !caller || !pLocalisation )
        return result;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller->GetPion().GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetResourceNetworksInZone( result, *pLocalisation );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::GetResourceNetworksInZoneByType
// Created: ABR 2012-03-02
// -----------------------------------------------------------------------------
T_ResourceNetworkVector DEC_ResourceNetworkFunctions::GetResourceNetworksInZoneByType( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation )
{
    T_ResourceNetworkVector result;
    if( !caller || !pLocalisation )
        return result;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller->GetPion().GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetResourceNetworksInZone( result, *pLocalisation, type );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromObject
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromObject( boost::shared_ptr< DEC_Knowledge_Object > source, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( !source.get() || !source->IsValid() )
        return false;
    MIL_Object_ABC* objectSource = source->GetObjectKnown();
    if( !objectSource )
        return false;
    return DoCreateResourceNetworkLink( *objectSource, target );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromUrbanBlock
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromUrbanBlock( MIL_UrbanObject_ABC* source, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( !source )
        return false;
    return DoCreateResourceNetworkLink( *source, target );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DoCreateResourceNetworkLink
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::DoCreateResourceNetworkLink( MIL_Object_ABC& objectSource, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( !target.get() )
        return false;
    ResourceNetworkCapacity* capacitySource = objectSource.Retrieve< ResourceNetworkCapacity >();
    if( !capacitySource )
        return false;
    const PHY_ResourceNetworkType* resourceType = PHY_ResourceNetworkType::Find( target->GetResource() );
    if( !resourceType )
        return false;
    MIL_Object_ABC* objectTarget = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( target->GetObjectId() );
    if( !objectTarget || !objectTarget->Retrieve< ResourceNetworkCapacity >())
        return false;
    capacitySource->CreateLink( objectTarget->GetID(), resourceType->GetId(), 0 );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DestroyResourceNetworkLink
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::DestroyResourceNetworkLink( boost::shared_ptr< DEC_ResourceNetwork > source, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( GetCapacity( target ) )
        if( ResourceNetworkCapacity* capacitySource = GetCapacity( source ) )
        {
            if( source->GetResource() != target->GetResource() )
                return false;
            return capacitySource->DestroyLink( target->GetObjectId(), target->GetResource() );
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateNode
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::CreateNode( boost::shared_ptr< DEC_Knowledge_Object > pObject, PHY_ResourceNetworkType* resourceType )
{
    if( !pObject.get() || !pObject->IsValid() || !resourceType )
        return false;
    MIL_Object_ABC* object = pObject->GetObjectKnown();
    if( !object )
        return false;
    ResourceNetworkCapacity* capacity = object->Retrieve< ResourceNetworkCapacity >();
    if( !capacity )
        return false;
    capacity->CreateNode( resourceType->GetId() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::GetSupply
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::GetSupply( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    ResourceNetworkCapacity* capacity = GetCapacity( resourceNetwork );
    if( !capacity || !callerAgent )
        return false;
    PHY_RoleInterface_Supply* supplyRole = callerAgent->GetPion().RetrieveRole< PHY_RoleInterface_Supply >();
    const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() );
    if( !supplyRole || !type )
        return false;
    const PHY_DotationCategory& dotation = type->GetDotationCategory();
    PHY_DotationStock* stock = supplyRole->GetStock( dotation );
    if( !stock )
        return false;
    double needed = stock->GetCapacity() - stock->GetValue();
    stock->Supply( - capacity->AddToStock( dotation, - needed ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::Supply
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::Supply( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    ResourceNetworkCapacity* capacity = GetCapacity( resourceNetwork );
    if( !capacity || !callerAgent )
        return false;
    PHY_RoleInterface_Supply* supplyRole = callerAgent->GetPion().RetrieveRole< PHY_RoleInterface_Supply >();
    const PHY_ResourceNetworkType* type = PHY_ResourceNetworkType::Find( resourceNetwork->GetResource() );
    if( !supplyRole || !type )
        return false;
    const PHY_DotationCategory& dotation = type->GetDotationCategory();
    PHY_DotationStock* stock = supplyRole->GetStock( dotation );
    if( !stock )
        return false;
    stock->Supply( -capacity->AddToStock( dotation, stock->GetValue() ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromResourceNetwork
// Created: DGE 2012-06-27
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork > source, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( !source.get() )
        return false;
    if( source->GetResource() != target->GetResource() )
        return false;
    MIL_Object_ABC* objectSource = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( source->GetObjectId() );
    if( !objectSource )
        return false;
    return DoCreateResourceNetworkLink( *objectSource, target );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetworkFunctions::DestroyResourceNetworkLinkWithObject
// Created: DGE 2012-07-03
// -----------------------------------------------------------------------------
bool DEC_ResourceNetworkFunctions::DestroyResourceNetworkLinkWithObject( MIL_Object_ABC& objectSource, boost::shared_ptr< DEC_ResourceNetwork > target )
{
    if( !target.get() )
        return false;
    MIL_Object_ABC* objectTarget = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( target->GetObjectId() );
    if( !objectTarget || !objectTarget->Retrieve< ResourceNetworkCapacity >())
        return false;
    ResourceNetworkCapacity* capacitySource = objectSource.Retrieve< ResourceNetworkCapacity >();
    if( !capacitySource )
        return false;
    return capacitySource->DestroyLink( target->GetObjectId(), target->GetResource() );
}
