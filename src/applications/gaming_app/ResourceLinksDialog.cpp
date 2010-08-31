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
#include "resource_network/Types.h"
#include <qtable.h>

using namespace actions;
using namespace kernel;
using namespace parameters;
using namespace resource;

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
    for( int i = 0; i < eNbrResourceType; ++i )
    {
        E_ResourceType type = static_cast< E_ResourceType >( i );
        const ResourceNetwork_ABC::ResourceNode* node = selected_->FindResourceNode( type );
        if( node )
        {
            ParameterList& node = nodes->AddList( "Node" );
            node.AddIdentifier( "Resource", i );
            node.AddQuantity( "Consumption", widgets_[ i ].consumption_->value() );
            node.AddBool( "Critical", widgets_[ i ].critical_->isChecked() );
            node.AddBool( "Enabled", widgets_[ i ].groupBox_->isChecked() );
            node.AddQuantity( "Production", widgets_[ i ].production_->value() );
            node.AddQuantity( "Stock", widgets_[ i ].stock_->value() );
            ParameterList& links = node.AddList( "Links" );
            for( int j = 0; j < widgets_[ i ].table_->numRows(); ++j )
            {
                int capacity = -1;
                if( static_cast< QCheckTableItem* >( widgets_[ i ].table_->item( j, 1 ) )->isChecked() )
                {
                    bool ok;
                    int newCapacity = widgets_[ i ].table_->text( j, 2 ).toInt( &ok );
                    if( ok )
                        capacity = newCapacity;
                }
                ParameterList& link = links.AddList( "Link" );
                link.AddIdentifier( "Link", j );
                link.AddQuantity( "Capacity", capacity );
            }
        }        
    }
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
}
