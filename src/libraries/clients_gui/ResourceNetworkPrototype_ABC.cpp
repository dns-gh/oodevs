// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::ResourceNetworkPrototype_ABC */

#include "clients_gui_pch.h"
#include "ResourceNetworkPrototype_ABC.h"
#include "Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype_ABC constructor
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
ResourceNetworkPrototype_ABC::ResourceNetworkPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::ResourceNetworkPrototype_ABC", "Resource network" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype_ABC destructor
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
ResourceNetworkPrototype_ABC::~ResourceNetworkPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkPrototype_ABC::CheckValidity
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
bool ResourceNetworkPrototype_ABC::CheckValidity() const
{
    return true;
}
