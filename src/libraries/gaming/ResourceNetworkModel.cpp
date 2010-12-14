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
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ResourceNetworkSelectionObserver.h"
#include "StructuralStateAttribute.h"
#include "protocol/Protocol.h"

#pragma warning( disable : 4706 )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel, tools::Resolver< gui::TerrainObjectProxy >& urbanObjects )
    : controllers_ ( controllers )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , urbanObjects_( urbanObjects )
    , observer_( *new kernel::ResourceNetworkSelectionObserver( controllers ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel destructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::~ResourceNetworkModel()
{
    delete &observer_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel DoUpdate
// Created: MGD 2010-11-25
// -----------------------------------------------------------------------------
void ResourceNetworkModel::DoUpdate( const sword::UrbanUpdate& message )
{
    if( message.has_attributes() )
    {
        gui::TerrainObjectProxy* pTerrainObject = urbanObjects_.Find( message.urban_object().id() );
        if( pTerrainObject )
        {
            if( message.attributes().has_infrastructures() && pTerrainObject->Retrieve< kernel::ResourceNetwork_ABC >() == 0 )
            {
                if( message.attributes().infrastructures().resource_network_size() > 0 )
                    Create( *pTerrainObject, message.attributes().infrastructures() );
                // TODO update infrastructures other than resource network
            }
        }
    }
}

