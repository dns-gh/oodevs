// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ResourceNetworkAttribute.h"
#include "MIL_Object_ABC.h"
#include "ResourceNetworkCapacity.h"
#include "CheckPoints/SerializationTools.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"
#include "resource_network/NodeProperties.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ResourceNetworkAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< ResourceNetworkAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< ResourceNetworkAttribute > )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( MIL_Object_ABC& object )
{
    ResourceNetworkCapacity* capacity = object.Retrieve< ResourceNetworkCapacity >();
    if( capacity )
        nodeProperties_ = capacity->GetNodeProperties();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( xml::xistream& xis, MIL_Object_ABC& object )
{
    ResourceNetworkCapacity* capacity = object.Retrieve< ResourceNetworkCapacity >();
    if( capacity )
    {
        nodeProperties_ = capacity->GetNodeProperties();
        capacity->Update( xis, object );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute destructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::~ResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::operator=
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute& ResourceNetworkAttribute::operator=( const ResourceNetworkAttribute& from )
{
    nodeProperties_ = from.nodeProperties_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Instanciate
// Created: JSR 2011-12-15
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< ResourceNetworkAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2011-12-15
// -----------------------------------------------------------------------------
bool ResourceNetworkAttribute::Update( const ResourceNetworkAttribute& rhs )
{
    nodeProperties_ = rhs.nodeProperties_;
    return nodeProperties_.get() ? nodeProperties_->NeedUpdate() : false;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SendFullState
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( nodeProperties_.get() )
        nodeProperties_->Serialize( *asn.mutable_resource_networks() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SendUpdate
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
bool ResourceNetworkAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( nodeProperties_.get() && nodeProperties_->NeedUpdate() )
    {
        nodeProperties_->Serialize( *asn.mutable_resource_networks() );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::serialize
// Created: JSR 2011-06-15
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceNetworkAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & nodeProperties_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::WriteODB
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::WriteODB( xml::xostream& xos ) const
{
    if( nodeProperties_ )
    {
        xos << xml::start( "resources" );
        nodeProperties_->WriteODB( xos );
        xos << xml::end;
    }
}

