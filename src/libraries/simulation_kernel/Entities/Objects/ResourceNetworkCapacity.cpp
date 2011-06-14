// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ResourceNetworkCapacity.h"
#include "ResourceNetworkAttribute.h"
#include "MIL_Object_ABC.h"
#include "MIL_AgentServer.h"
#include "StructuralCapacity.h"
#include "protocol/Protocol.h"
#include "resource_network/NodeProperties.h"
#include "resource_network/ResourceNetworkModel.h"
#include "PHY_ResourceNetworkType.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

BOOST_CLASS_EXPORT_IMPLEMENT( ResourceNetworkCapacity )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity constructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity()
    : nodeProperties_( new NodeProperties( MIL_AgentServer::GetWorkspace().GetResourceTools() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity constructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity( xml::xistream& xis )
    : nodeProperties_( new NodeProperties( xis, MIL_AgentServer::GetWorkspace().GetResourceTools() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity constructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity( const ResourceNetworkCapacity& from )
    : nodeProperties_ ( new NodeProperties( *from.nodeProperties_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity destructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::~ResourceNetworkCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Update
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Update( xml::xistream& xis, const MIL_Object_ABC& /*object*/ )
{
    nodeProperties_->Update( xis );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Update
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Initialize( const urban::ResourceNetworkAttribute& urbanAttribute )
{
    nodeProperties_->Initialize( urbanAttribute );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Update( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list )
{
    nodeProperties_->Update( list );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::load
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    NodeProperties* nodeProperties;
    file >> boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         >> nodeProperties;
    nodeProperties_.reset( nodeProperties );
    nodeProperties_->SetTools( MIL_AgentServer::GetWorkspace().GetResourceTools() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::save
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    NodeProperties* nodeProperties = nodeProperties_.get();
    file << boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         << nodeProperties;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Register
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( *static_cast< MIL_StructuralStateNotifier_ABC *>( this ) );
    RegisterNode( object.GetID() );
    // $$$$ JSR 2011-01-14: TODO vérifier si ça peut être fait automatiquement par MIL_StructuralStateNotifier_ABC
    if( StructuralCapacity* structural = object.Retrieve< StructuralCapacity >() )
        NotifyStructuralStateChanged( structural->GetStructuralState(), object );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Instanciate
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ResourceNetworkCapacity* capacity = new ResourceNetworkCapacity( *this );
    object.AddCapacity( capacity );
    object.GetAttribute< ResourceNetworkAttribute >() = ResourceNetworkAttribute( object );
    object.Register( *static_cast< MIL_StructuralStateNotifier_ABC *>( capacity ) );
    capacity->RegisterNode( object.GetID() );
    // $$$$ JSR 2011-01-14: TODO vérifier si ça peut être fait automatiquement par MIL_StructuralStateNotifier_ABC
    if( StructuralCapacity* structural = object.Retrieve< StructuralCapacity >() )
        nodeProperties_->SetModifier( structural->GetStructuralState() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::RegisterNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::RegisterNode( unsigned int id )
{
    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().RegisterNode( *nodeProperties_, id );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendState
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendState( sword::UrbanAttributes& message ) const
{
    if( nodeProperties_->NeedUpdate() )
        nodeProperties_->Serialize( *message.mutable_infrastructures() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendState
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendState( sword::ObjectAttributes& asn ) const
{
    if( nodeProperties_->NeedUpdate() )
        nodeProperties_->Serialize( *asn.mutable_resource_networks() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendFullState
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendFullState( sword::UrbanAttributes& message ) const
{
    nodeProperties_->Serialize( *message.mutable_infrastructures() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendFullState
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendFullState( sword::ObjectAttributes& asn ) const
{
    nodeProperties_->Serialize( *asn.mutable_resource_networks() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::NotifyStructuralStateChanged
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& /*object*/ )
{
    nodeProperties_->SetModifier( structuralState );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::ActivateAll
// Created: JSR 2011-05-24
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::ActivateAll()
{
    nodeProperties_->ActivateAll();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SetActivation
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SetActivation( unsigned long resourceId, bool activated )
{
    nodeProperties_->SetActivation( resourceId, activated );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::CreateLink
// Created: GGE 2011-06-10
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::CreateLink( unsigned long targetId, unsigned long resourceId, unsigned int production )
{
    nodeProperties_->CreateLink( targetId, resourceId, production );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::AddConsumption
// Created: JSR 2011-02-01
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::AddConsumption( unsigned long resourceId, double consumption )
{
    nodeProperties_->AddConsumption( resourceId, consumption );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::GetConsumptionState
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
float ResourceNetworkCapacity::GetConsumptionState( unsigned long resourceId ) const
{
    return nodeProperties_->GetConsumptionState( resourceId );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::GetFunctionalState
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
float ResourceNetworkCapacity::GetFunctionalState() const
{
    return nodeProperties_->GetFunctionalState();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::AddToStock
// Created: BCI 2011-02-21
// -----------------------------------------------------------------------------
double ResourceNetworkCapacity::AddToStock( const PHY_DotationCategory& dotation, double quantity )
{
    if( const PHY_ResourceNetworkType* pType = PHY_ResourceNetworkType::FindByDotation( dotation ) )
        return nodeProperties_->AddToStock( pType->GetId(), quantity );
    else
        return 0.;
}
