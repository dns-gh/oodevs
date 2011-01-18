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
#include "MIL_Object_ABC.h"
#include "MIL_AgentServer.h"
#include "StructuralCapacity.h"
#include "UrbanObjectWrapper.h"
#include "protocol/Protocol.h"
#include "resource_network/NodeProperties.h"
#include "resource_network/ResourceNetworkModel.h"
#include <urban/TerrainObject_ABC.h>
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
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity( const urban::ResourceNetworkAttribute& urbanAttribute )
    : nodeProperties_( new NodeProperties( urbanAttribute, MIL_AgentServer::GetWorkspace().GetResourceTools() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity constructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity( const ResourceNetworkCapacity& from )
{
    nodeProperties_ = new NodeProperties( *from.nodeProperties_ );
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
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
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
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().RegisterNode( *nodeProperties_, id );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendState
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendState( sword::UrbanAttributes& message )
{
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    if( nodeProperties_->NeedUpdate() )
        nodeProperties_->Serialize( *message.mutable_infrastructures() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::SendState
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SendState( sword::ObjectAttributes& asn ) const
{
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    if( nodeProperties_->NeedUpdate() )
        nodeProperties_->Serialize( *asn.mutable_resource_networks() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::NotifyStructuralStateChanged
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::NotifyStructuralStateChanged( unsigned int structuralState, const MIL_Object_ABC& /*object*/ )
{
    nodeProperties_->SetModifier( structuralState );
}
