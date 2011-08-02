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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "preparation/ResourceNetworkAttribute.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, const tools::StringResolver< kernel::ResourceNetworkType >& resources )
    : gui::ResourceLinksDialog_ABC( parent, controllers, resources )
{
    setObjectName( "ressources" );
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
