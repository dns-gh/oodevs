// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticMagicInterface.h"
#include "moc_LogisticMagicInterface.cpp"

#include "LogisticSupplyChangeQuotasDialog.h"
#include "LogisticSupplyPullFlowDialog.h"
#include "LogisticSupplyPushFlowDialog.h"

#include "actions/ActionsModel.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/AgentsModel.h"
#include "gaming/Model.h"
#include "gaming/TeamsModel.h"

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface constructor
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
LogisticMagicInterface::LogisticMagicInterface( QWidget* parent,
                                                kernel::Controllers& controllers,
                                                Model& model,
                                                const StaticModel& staticModel,
                                                const kernel::Time_ABC& simulation,
                                                const kernel::Profile_ABC& profile,
                                                gui::ParametersLayer& layer )
    : QObject( parent )
    , controllers_( controllers )
    , actionsModel_( model.actions_ )
    , selected_( controllers.controller_ )
    , profile_( profile )
    , changeQuotasDialog_( new LogisticSupplyChangeQuotasDialog( parent,
                                                                 controllers,
                                                                 model.actions_,
                                                                 staticModel,
                                                                 simulation,
                                                                 model ) )
    , pushFlowDialog_( new LogisticSupplyPushFlowDialog( parent,
                                                         controllers,
                                                         model.actions_,
                                                         staticModel,
                                                         simulation,
                                                         layer,
                                                         model.agents_,
                                                         profile ) )
    , pullFlowDialog_( new LogisticSupplyPullFlowDialog( parent,
                                                         controllers,
                                                         model.actions_,
                                                         staticModel,
                                                         simulation,
                                                         layer,
                                                         model.agents_,
                                                         model.teams_,
                                                         profile ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface destructor
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
LogisticMagicInterface::~LogisticMagicInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::NotifyContextMenu
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( !profile_.CanBeOrdered( automat ) || !automat.Get< gui::LogisticBase >().IsBase() )
        return;
    selected_ = &automat;
    AddMenuEntries( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::NotifyContextMenu
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( !profile_.CanBeOrdered( formation ) || !formation.Get< gui::LogisticBase >().IsBase() )
        return;
    selected_ = &formation;
    AddMenuEntries( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::AddMenuEntries
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::AddMenuEntries( kernel::ContextMenu& menu )
{
    if( !selected_ )
        return;
    kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tr( "Logistic actions" ), false, 2 );
    subMenu->InsertItem( "Command", tr( "Allocate supply quotas" ), this, SLOT( OnChangeQuotas() ) );
    subMenu->InsertItem( "Command", tr( "Push supply flow" ), this, SLOT( OnPushFlow() ) );
    subMenu->InsertItem( "Command", tr( "Resupply" ), this, SLOT( OnResupply() ) );
    if( selected_->GetTypeName() == kernel::Automat_ABC::typeName_ )
        subMenu->InsertItem( "Command", tr( "Pull supply flow" ), this, SLOT( OnPullFlow() ) );
    if( profile_.IsSupervision() )
        subMenu->InsertItem( "Command",
            selected_->Get< gui::LogisticBase >().IsMaintenanceManual()
                ? tr( "Switch to automated maintenance" )
                : tr( "Switch to manual maintenance" ), this, SLOT( OnSwitchMaintenanceMode() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::OnChangeQuotas
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::OnChangeQuotas()
{
    if( !selected_ )
        return;
    changeQuotasDialog_->Show( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::OnPullFlow
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::OnPullFlow()
{
    if( !selected_ )
        return;
    pullFlowDialog_->Show( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::OnPushFlow
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::OnPushFlow()
{
    if( !selected_ )
        return;
    pushFlowDialog_->PushFlow( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::OnResupply
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::OnResupply()
{
    if( !selected_ )
        return;
    pushFlowDialog_->Supply( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::OnSwitchMaintenanceMode
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::OnSwitchMaintenanceMode()
{
    if( !selected_ )
        return;
    actionsModel_.PublishLogMaintenanceSetManualAction( *selected_,
                                                        !selected_->Get< gui::LogisticBase >().IsMaintenanceManual() );
}
