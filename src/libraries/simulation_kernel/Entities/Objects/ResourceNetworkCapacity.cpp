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
#include "Urban/MIL_UrbanObject_ABC.h"
#include "CheckPoints/SerializationTools.h"
#include "Decision/DEC_ResourceNetwork.h"
#include "protocol/Protocol.h"
#include "resource_network/NodeProperties.h"
#include "resource_network/ResourceNetworkModel.h"
#include "Urban/PHY_ResourceNetworkType.h"
#include <boost/make_shared.hpp>

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
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Update( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list )
{
    nodeProperties_->Update( list );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< resource::NodeProperties> ResourceNetworkCapacity::GetNodeProperties
// Created: JSR 2011-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< resource::NodeProperties> ResourceNetworkCapacity::GetNodeProperties() const
{
    return nodeProperties_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::load
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         >> nodeProperties_;
    nodeProperties_->SetTools( MIL_AgentServer::GetWorkspace().GetResourceTools() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::save
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         << nodeProperties_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::WriteUrban
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::WriteUrban( xml::xostream& xos ) const
{
    if( nodeProperties_ )
    {
        xos << xml::start( "resources" );
        nodeProperties_->WriteODB( xos );
        xos << xml::end;
    }
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
    if( dynamic_cast< MIL_UrbanObject_ABC* >( &object ) == 0 )
        object.GetAttribute< ResourceNetworkAttribute >() = ResourceNetworkAttribute( object );
    object.Register( *static_cast< MIL_StructuralStateNotifier_ABC *>( capacity ) );
    capacity->RegisterNode( object.GetID() );
    // $$$$ JSR 2011-01-14: TODO vérifier si ça peut être fait automatiquement par MIL_StructuralStateNotifier_ABC
    if( StructuralCapacity* structural = object.Retrieve< StructuralCapacity >() )
        nodeProperties_->SetModifier( structural->GetStructuralState() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Terminate
// Created: MCO 2012-11-09
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Terminate( MIL_Object_ABC& object ) const
{
    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().UnregisterNode( object.GetID() );
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
bool ResourceNetworkCapacity::SendState( sword::ObjectAttributes& asn ) const
{
    if( nodeProperties_->NeedUpdate() )
    {
        nodeProperties_->Serialize( *asn.mutable_resource_networks() );
        return true;
    }
    return false;
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
// Name: ResourceNetworkCapacity::CreateNode
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::CreateNode( unsigned long resourceId )
{
    nodeProperties_->CreateNode( resourceId );
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
// Name: ResourceNetworkCapacity::DestroyLink
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool ResourceNetworkCapacity::DestroyLink( unsigned long targetId, const std::string& resource )
{
    return nodeProperties_->DestroyLink( targetId, resource );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::AddProduction
// Created: GGE 2011-08-04
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::AddProduction( unsigned long resourceId, unsigned int production )
{
    nodeProperties_->AddProduction( resourceId, production );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::DecreaseProduction
// Created: GGE 2011-08-04
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::DecreaseProduction( unsigned long resourceId, unsigned int production )
{
    nodeProperties_->DecreaseProduction( resourceId, production );
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
// Name: ResourceNetworkCapacity::GetStock
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
double ResourceNetworkCapacity::GetStock( const PHY_DotationCategory& dotation ) const
{
    if( const PHY_ResourceNetworkType* pType = PHY_ResourceNetworkType::FindByDotation( dotation ) )
        return nodeProperties_->GetStock( pType->GetId() );
    else
        return 0.;
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

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::GetDECResourceNetworks
// Created: JSR 2012-01-17
// -----------------------------------------------------------------------------
const T_ResourceNetworkVector& ResourceNetworkCapacity::GetDECResourceNetworks( unsigned int objectId ) const
{
    std::vector< std::string > resources;
    nodeProperties_->GetAvailableResources( resources );
    if( resources.size() != DECResourceNetworks_.size() )
    {
        DECResourceNetworks_.clear();
        for( auto it = resources.begin(); it != resources.end(); ++it )
            DECResourceNetworks_.push_back( boost::make_shared< DEC_ResourceNetwork >( objectId, *it ) );
    }
    return DECResourceNetworks_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::ContainsType
// Created: ABR 2012-03-02
// -----------------------------------------------------------------------------
bool ResourceNetworkCapacity::ContainsType( const std::string& type ) const
{
    std::vector< std::string > availableResources;
    nodeProperties_->GetAvailableResources( availableResources );
    return std::find( availableResources.begin(), availableResources.end(), type ) != availableResources.end();
}
