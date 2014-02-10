// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CriticalIntelligenceDialog.h"
#include "moc_CriticalIntelligenceDialog.cpp"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/String.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_gui/CriticalIntelligence.h"
#include "gaming/Attributes.h"
#include "gaming/Population.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog constructor
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
CriticalIntelligenceDialog::CriticalIntelligenceDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                                        actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : gui::CriticalIntelligenceDialog( pParent, controllers )
    , static_      ( staticModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , actionsModel_( actionsModel )
    , selected_    ( controllers )
{
    selected_ = 0;
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog destructor
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
CriticalIntelligenceDialog::~CriticalIntelligenceDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::Validate
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::OnAccept()
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_critical_intelligence" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new actions::parameters::String( it.NextElement(), textEdit_->text().toStdString() ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );
    accept();
    textEdit_->setText( "" );
    gui::CriticalIntelligenceDialog::OnAccept();
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::Reject
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::OnReject()
{
    selected_ = 0;
    textEdit_->setText( "" );
    gui::CriticalIntelligenceDialog::OnReject();
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::NotifyContextMenu
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
        subMenu->insertItem( tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), this, SLOT( OnShow() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::NotifyContextMenu
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
        subMenu->insertItem( tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), this, SLOT( OnShow() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::OnShow
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::OnShow()
{
    if( !selected_ )
        return;
    if( gui::CriticalIntelligence* attribute = selected_.ConstCast()->Retrieve< gui::CriticalIntelligence >() )
        SetValue( attribute->GetType() );
    show();
}
