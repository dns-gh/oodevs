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
#include "actions/ActionTiming.h"
#include "actions/Bool.h"
#include "actions/Identifier.h"
#include "actions/ParameterList.h"
#include "actions/MagicAction.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "gaming/StaticModel.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog::ResourceLinksDialog( QMainWindow* parent, const kernel::Model_ABC& model, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : ResourceLinksDialog_ABC( parent, model, controllers, staticModel.objectTypes_ )
    , actionsModel_( actionsModel )
    , static_      ( staticModel )
    , simulation_  ( simulation )
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
void ResourceLinksDialog::DoValidate( kernel::Entity_ABC* element /*= 0*/ )
{
    if( element )
        actionsModel_.PublishChangeResourceLinks( element->GetId(), element->Get< gui::ResourceNetwork_ABC >().GetResourceNodes() );
    else
        actionsModel_.PublishChangeResourceLinks( id_, resourceNodes_ );
}
