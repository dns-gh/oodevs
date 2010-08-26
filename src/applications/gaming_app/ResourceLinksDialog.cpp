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
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "gaming/StaticModel.h"
#include <qtable.h>

using namespace actions;
using namespace kernel;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog::ResourceLinksDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : ResourceLinksDialog_ABC( parent, controllers, profile )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
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
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_resource_links" );
    MagicAction* action = new MagicAction( actionType, controllers_.controller_, tr( "Change resource links"), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new Identifier( it.NextElement(), id_ ) );
    action->AddParameter( *new Bool( it.NextElement(), urban_ ) );
    ParameterList* nodes = new ParameterList( it.NextElement() );
    action->AddParameter( *nodes );
    for( int i = 0; i < resource::eNbrResourceType; ++i )
    {
        resource::EResourceType type = static_cast< resource::EResourceType >( i );
        const ResourceNetwork_ABC::ResourceNode* node = selected_->FindResourceNode( type );
        if( node )
        {
            ParameterList& node = nodes->AddList( "Node" );
            node.AddIdentifier( "Resource", i );
            ParameterList& links = node.AddList( "Links" );
            for( int j = 0; j < table_[ i ]->numRows(); ++j )
            {
                bool ok;
                int capacity = table_[ i ]->text( j, 1 ).toInt( &ok );
                if( ok )
                {
                    ParameterList& link = links.AddList( "Link" );
                    link.AddIdentifier( "Link", j );
                    link.AddQuantity( "Capacity", capacity );
                }
            }
        }        
    }
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
}
