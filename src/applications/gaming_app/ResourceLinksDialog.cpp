// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ResourceLinksDialog.h"
#include "actions/ActionsModel.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "gaming/StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog::ResourceLinksDialog( QMainWindow* parent,
                                          const Model_ABC& model,
                                          Controllers& controllers,
                                          actions::ActionsModel& actionsModel,
                                          const ::StaticModel& staticModel )
    : ResourceLinksDialog_ABC( parent, model, controllers, staticModel.objectTypes_ )
    , actionsModel_( actionsModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog destructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog::~ResourceLinksDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog::DoValidate
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void ResourceLinksDialog::DoValidate( Entity_ABC* element /*= 0*/ )
{
    if( element )
        actionsModel_.PublishChangeResourceLinks( element->GetId(), element->Get< gui::ResourceNetwork_ABC >().GetResourceNodes() );
    else
        actionsModel_.PublishChangeResourceLinks( id_, resourceNodes_ );
}
