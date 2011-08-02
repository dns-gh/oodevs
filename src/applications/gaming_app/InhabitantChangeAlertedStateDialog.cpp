// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeAlertedStateDialog.h"
#include "moc_InhabitantChangeAlertedStateDialog.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/UnitMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/tools.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAlertedStateDialog::InhabitantChangeAlertedStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantChangeAlertedStateDialog", "Change alert state" ), true )
    , controllers_  ( controllers )
    , actionsModel_ ( actionsModel )
    , simulation_   ( simulation )
    , profile_      ( profile )
    , selected_     ( controllers )
{
    // Init dialog
    setCaption( tools::translate( "InhabitantChangeAlertedStateDialog", "Change alert state" ) );
    resize( 320, 150 );
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    pAlertedCheckBox_ = new QCheckBox( tools::translate( "InhabitantChangeAlertedStateDialog", "Alerted" ), this );
    mainLayout->addWidget( pAlertedCheckBox_ );
    // ok / cancel butons
    Q3HBox* buttonLayout = new Q3HBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    okButton->setDefault( TRUE );
    mainLayout->addWidget( buttonLayout );
    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAlertedStateDialog::~InhabitantChangeAlertedStateDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAlertedStateDialog::Show()
{
    if( !selected_ )
        return;
    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAlertedStateDialog::Validate()
{
    if( ! selected_ )
        return;
    accept();

    actions::Action_ABC* action = actionsModel_.CreateInhabitantChangeAlertedStateAction( pAlertedCheckBox_->isChecked(), *selected_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAlertedStateDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAlertedStateDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAlertedStateDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAlertedStateDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        Q3PopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantChangeAlertedStateDialog", "Change alert state" ), this, SLOT( Show() ) );
    }
}
