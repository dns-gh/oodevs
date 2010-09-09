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
#include "preparation/ResourceNetworkAttribute.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile )
    : gui::ResourceLinksDialog_ABC( parent, controllers, dotationResolver, profile )
{
    stockBox_->show();
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
    static_cast< ResourceNetworkAttribute* >( const_cast< kernel::ResourceNetwork_ABC* >( selected_ ) )->Update( resourceNodes_ );
}
