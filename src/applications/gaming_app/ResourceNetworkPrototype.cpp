// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ResourceNetworkPrototype.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype constructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
ResourceNetworkPrototype::ResourceNetworkPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : ResourceNetworkPrototype_ABC( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype destructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
ResourceNetworkPrototype::~ResourceNetworkPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype::Commit
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
void ResourceNetworkPrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Resource" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::resource_network );
}
