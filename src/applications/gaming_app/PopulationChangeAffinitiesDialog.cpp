// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationChangeAffinitiesDialog.h"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Affinities.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: PopulationChangeAffinitiesDialog constructor
// Created: LGY 2011-01-25
// -----------------------------------------------------------------------------
PopulationChangeAffinitiesDialog::PopulationChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                                                    actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : ChangeAffinitiesDialog( pParent, controllers )
    , controllers_ ( controllers )
    , static_      ( staticModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , actionsModel_( actionsModel )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeAffinitiesDialog destructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
PopulationChangeAffinitiesDialog::~PopulationChangeAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeAffinitiesDialog::DoValidate
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void PopulationChangeAffinitiesDialog::DoValidate()
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "crowd_change_affinities" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* affinitiesList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *affinitiesList );
    selected_->Get< Affinities >().FillParameterList( affinitiesList );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void PopulationChangeAffinitiesDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
        subMenu->insertItem( tools::translate( "ChangeAffinitiesDialog", "Change affinities" ), this, SLOT( Show() ) );
    }
}
