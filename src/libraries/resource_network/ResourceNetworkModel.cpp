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
// Name: ResourceNetworkModel::FindResourceType
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
EResourceType ResourceNetworkModel::FindResourceType( const std::string& type )
{
    if( type == "water" )
        return eResourceTypeWater;
    if( type == "gaz" )
        return eResourceTypeGaz;
    if( type == "electricity" )
        return eResourceTypeElectricity;
    throw std::exception( std::string( "Unknown resource type : " + type ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Update()
{
    Apply( boost::bind( &NodeProperties::Update, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::RegisterNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void ResourceNetworkModel::RegisterNode( NodeProperties& nodeProperties, unsigned int id )
{
    Register( id, nodeProperties );
    nodeProperties.SetModel( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Push( unsigned int id, int quantity, EResourceType type ) const
{
    NodeProperties* node = Find( id );
    if( !node )
        throw std::exception( "Node not found" );
    node->Push( quantity, type );
}
