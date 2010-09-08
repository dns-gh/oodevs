// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ResourceNetworkDialog.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile )
    : gui::ResourceLinksDialog_ABC( parent, controllers, dotationResolver, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog destructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::~ResourceNetworkDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::DoValidate
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkDialog::DoValidate()
{
    const_cast< kernel::ResourceNetwork_ABC* >( selected_ )->GetResourceNodes() = resourceNodes_;
}
