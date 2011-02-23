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
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Finalize
// Created: JSR 2011-01-17
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Finalize()
{
    Apply( boost::bind( &NodeProperties::Finalize, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Update()
{
    Apply( boost::bind( &NodeProperties::UpdateState, _1, boost::cref( *this ) ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::RegisterNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void ResourceNetworkModel::RegisterNode( NodeProperties& nodeProperties, unsigned int id )
{
    Register( id, nodeProperties );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Push( unsigned int id, int quantity, unsigned long resourceId ) const
{
    NodeProperties* node = Find( id );
    if( !node )
        throw std::exception( "Node not found" );
    node->Push( quantity, resourceId );
}
