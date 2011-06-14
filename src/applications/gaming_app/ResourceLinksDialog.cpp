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
#include "actions/ActionTiming.h"
#include "actions/Bool.h"
#include "actions/Identifier.h"
#include "actions/ParameterList.h"
#include "actions/MagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "gaming/StaticModel.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog::ResourceLinksDialog( QMainWindow* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : ResourceLinksDialog_ABC( parent, controllers, staticModel.objectTypes_ )
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
void ResourceLinksDialog::DoValidate()
{
    // $$$$ _RC_ JSR 2011-02-24: TODO passer dans la factory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_resource_links" );
    MagicAction* action = new MagicAction( actionType, controllers_.controller_, tr( "Change resource links"), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new Identifier( it.NextElement(), id_ ) );
    ParameterList* nodes = new ParameterList( it.NextElement() );
    action->AddParameter( *nodes );
    for( ResourceNetwork_ABC::CIT_ResourceNodes it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
    {
        const ResourceNetwork_ABC::ResourceNode& resource = it->second;
        ParameterList& node = nodes->AddList( "Node" );
        node.AddString( "Resource", resource.resource_ );
        node.AddQuantity( "Consumption", resource.consumption_ );
        node.AddBool( "Critical", resource.critical_ );
        node.AddBool( "Enabled", resource.isEnabled_ );
        node.AddQuantity( "Production", resource.production_ );
        node.AddQuantity( "MaxStock", resource.maxStock_ );
        ParameterList& links = node.AddList( "Links" );
        for( unsigned int i = 0; i < resource.links_.size(); ++i )
        {
            ParameterList& link = links.AddList( "Link" );
            link.AddIdentifier( "Link", resource.links_[ i ].id_ );
            link.AddQuantity( "Capacity", resource.links_[ i ].capacity_ );
        }
    }
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->RegisterAndPublish( actionsModel_ );
}
