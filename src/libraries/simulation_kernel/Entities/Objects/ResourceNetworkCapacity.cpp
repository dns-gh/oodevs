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
#include "UrbanObjectWrapper.h"
#include "protocol/protocol.h"
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
    : nodeProperties_( new NodeProperties )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity constructor
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
ResourceNetworkCapacity::ResourceNetworkCapacity( xml::xistream& xis )
    : nodeProperties_( new NodeProperties( xis ) )
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
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Update
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Update( xml::xistream& xis )
{
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    nodeProperties_->Update( xis );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::serialize
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceNetworkCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    //TODO ?
         //& nodeProperties_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Register
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    // $$$$ JSR 2010-08-13: TODO Am�liorer quand on g�rera les objets
    UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( wrapper )
        RegisterNode( wrapper->GetObject().GetId() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkCapacity::Instanciate
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void ResourceNetworkCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ResourceNetworkCapacity* capacity = new ResourceNetworkCapacity( *this );
    object.AddCapacity( capacity );
    // $$$$ JSR 2010-08-13: TODO Am�liorer quand on g�rera les objets
    UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( wrapper )
        capacity->RegisterNode( wrapper->GetObject().GetId() );
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
void ResourceNetworkCapacity::SendState( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    if( nodeProperties_ == 0 )
        throw std::exception( "RegisterResource : Node Properties not instanciated" );
    nodeProperties_->Serialize( *message.mutable_infrastructures() );
}
