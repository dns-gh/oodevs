// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ResourceNetworkModel.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkFactory.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel( ResourceNetworkFactory& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel destructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::~ResourceNetworkModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::UrbanCreate
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetworkModel::UrbanCreate( kernel::Entity_ABC& entity, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg )
{
    ResourceNetwork* element = factory_.Create( entity.GetId(), msg, true );
    entity.Attach< ResourceNetwork >( *element );
    UrbanResourceResolver::Register( entity.GetId(), *element );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::UrbanUpdate
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
void ResourceNetworkModel::UrbanUpdate( unsigned int id, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg )
{
    UrbanResourceResolver::Get( id ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Purge
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Purge()
{
    // Do not use DeleteAll : pointers will be deleted by the Entity to which they are attached
    UrbanResourceResolver::Clear();
    ObjectResourceResolver::Clear();
}
