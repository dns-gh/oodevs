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
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog constructor
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
CriticalIntelligenceDialog::CriticalIntelligenceDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                                       actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), true )
    , controllers_ ( controllers )
    , static_      ( staticModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , actionsModel_( actionsModel )
    , selected_    ( controllers )
{
    setCaption( tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ) );
    resize( 240, 70 );
    QGridLayout* pLayout = new QGridLayout( this, 2, 2, 4 );
    pLayout->addWidget( new QLabel( tr( "Critical intelligence:" ), this ), 1, 0 );
    pLayout->setRowStretch( 6, 1 );
    pLayout->setRowStretch( 2, 0 );
    value_ = new QLineEdit( this );
    pLayout->addWidget( value_, 1, 1 );
    QHBox* buttonLayout = new QHBox( this );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    okButton->setDefault( TRUE );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    pLayout->addMultiCellWidget( buttonLayout, 2, 2, 0, 1, Qt::AlignCenter );
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
void CriticalIntelligenceDialog::Validate()
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_critical_intelligence" );
    actions::UnitMagicAction* action = new actions::UnitMagicAction( *selected_, actionType, controllers_.controller_, tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new actions::parameters::String( it.NextElement(), value_->text().ascii() ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
    accept();
    value_->setText( "" );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::Reject
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::Reject()
{
    reject();
    selected_ = 0;
    value_->setText( "" );
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
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), this, SLOT( show() ) );
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
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "CriticalIntelligenceDialog", "Change critical intelligence" ), this, SLOT( show() ) );
    }
}
