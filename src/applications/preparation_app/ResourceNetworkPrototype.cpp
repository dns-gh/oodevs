// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ResourceNetworkPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/ResourceNetworkAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype constructor
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
ResourceNetworkPrototype::ResourceNetworkPrototype( QWidget* parent, Object_ABC*& creation, kernel::Controllers& controllers
                                                  , const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbans
                                                  , const tools::Resolver_ABC< kernel::Object_ABC >& objects
                                                  , const tools::StringResolver< kernel::ResourceNetworkType >& resources )
    : ResourceNetworkPrototype_ABC( parent )
    , creation_                   ( creation )
    , controllers_                ( controllers )
    , urbans_                     ( urbans )
    , objects_                    ( objects )
    , resources_                  ( resources )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype destructor
// Created: JSR 2010-02-23
// -----------------------------------------------------------------------------
ResourceNetworkPrototype::~ResourceNetworkPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype::Commit
// Created: JSR 2010-02-23
// -----------------------------------------------------------------------------
void ResourceNetworkPrototype::Commit()
{
    if( creation_ )
        creation_->Attach< ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, creation_->Get< Positions>(), urbans_, objects_, resources_ ) );
}
