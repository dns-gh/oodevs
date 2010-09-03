// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ResourceNetworkModel.h"
#include <boost/bind.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel destructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceNetworkModel::~ResourceNetworkModel()
{
    UrbanResolver::DeleteAll();
    ObjectResolver::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Update()
{
    UrbanResolver::Apply( boost::bind( &NodeProperties::Update, _1 ) );
    ObjectResolver::Apply( boost::bind( &NodeProperties::Update, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::RegisterNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void ResourceNetworkModel::RegisterNode( NodeProperties& nodeProperties, unsigned int id, bool urban )
{
    if( urban )
        UrbanResolver::Register( id, nodeProperties );
    else
        ObjectResolver::Register( id, nodeProperties );
    nodeProperties.SetModel( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Push( unsigned int id, bool urban, int quantity, unsigned long resourceId ) const
{
    NodeProperties* node = urban ? UrbanResolver::Find( id ) : ObjectResolver::Find( id );
    if( !node )
        throw std::exception( "Node not found" );
    node->Push( quantity, resourceId );
}
