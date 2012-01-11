// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeAffinitiesDialog.h"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Affinities.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog constructor
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
InhabitantChangeAffinitiesDialog::InhabitantChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
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
// Name: InhabitantChangeAffinitiesDialog destructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
InhabitantChangeAffinitiesDialog::~InhabitantChangeAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "ChangeAffinitiesDialog", "Change affinities" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::DoValidate
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::DoValidate()
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "inhabitant_change_affinities" );
    actions::UnitMagicAction* action = new actions::UnitMagicAction( *selected_, actionType, controllers_.controller_, tools::translate( "ChangeAffinitiesDialog", "Change affinities" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* affinitiesList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *affinitiesList );
    selected_->Get< Affinities >().FillParameterList( affinitiesList );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

