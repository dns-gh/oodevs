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
#include "clients_kernel/ResourceNetworkSelectionObserver.h"
#include "protocol/protocol.h"

#pragma warning( disable : 4706 )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_      ( model )
    , staticModel_( staticModel )
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

