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
#include "protocol/protocol.h"

#pragma warning( disable : 4706 )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel( kernel::Controllers& controllers, const Model& model )
    : controllers_( controllers )
    , model_      ( model )
    , selected_   ( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel destructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::~ResourceNetworkModel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Select
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetworkModel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ != 0 )
    {
        selected_->Select( false );
        selected_ = 0;
    }
    if( element && ( selected_ = const_cast< kernel::ResourceNetwork_ABC* >( element->Retrieve< kernel::ResourceNetwork_ABC >() ) ) )
        selected_->Select( true );
}
