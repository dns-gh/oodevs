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
#include "clients_gui/ResourceNetworkSelectionObserver.h"
#include "clients_gui/PropertiesDictionary.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel constructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::ResourceNetworkModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel )
    : controllers_ ( controllers )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , observer_    ( new gui::ResourceNetworkSelectionObserver( controllers ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel destructor
// Created: JSR 2010-08-18
// -----------------------------------------------------------------------------
ResourceNetworkModel::~ResourceNetworkModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel::Create
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
void ResourceNetworkModel::Create( kernel::Entity_ABC& entity )
{
    gui::PropertiesDictionary* dico = entity.Retrieve< gui::PropertiesDictionary >();
    ResourceNetwork* element = new ResourceNetwork( controllers_, entity, model_.urbanObjects_, model_.objects_, staticModel_.objectTypes_, dico );
    entity.Attach< gui::ResourceNetwork_ABC >( *element );
}
