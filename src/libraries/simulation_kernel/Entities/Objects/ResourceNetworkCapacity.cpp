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
    // TODO Find a better way
    UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( wrapper )
        RegisterNode( wrapper->GetObject().GetId(), true );
    else
        RegisterNode( object.GetID(), false );
    if( StructuralCapacity* structural = object.Retrieve< StructuralCapacity >() )
        SetModifier( structural->GetStructuralState() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Instanciate
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ResourceNetworkCapacity* capacity = new ResourceNetworkCapacity( *this );
    object.AddCapacity( capacity );
    // TODO Find a better way
    UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( wrapper )
        capacity->RegisterNode( wrapper->GetObject().GetId(), true );
    else
        capacity->RegisterNode( object.GetID(), false );
    if( StructuralCapacity* structural = object.Retrieve< StructuralCapacity >() )
        SetModifier( structural->GetStructuralState() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::RegisterNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::RegisterNode( unsigned int id, bool urban )
{
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().RegisterNode( *nodeProperties_, id, urban );
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
// Name: ResourceNetworkCapacity::SetModifier
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::SetModifier( unsigned int modifier ) const
{
    nodeProperties_->SetModifier( modifier );
}
