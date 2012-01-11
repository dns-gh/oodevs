// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeConfinedStateDialog.h"
#include "moc_InhabitantChangeConfinedStateDialog.cpp"
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
// Name: InhabitantChangeConfinedStateDialog constructor
// Created: BCI 2011-02-22
// -----------------------------------------------------------------------------
InhabitantChangeConfinedStateDialog::InhabitantChangeConfinedStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantChangeConfinedStateDialog", "Change confine state" ) )
    , controllers_  ( controllers )
    , actionsModel_ ( actionsModel )
    , simulation_   ( simulation )
    , profile_      ( profile )
    , selected_     ( controllers )
{
    // Init dialog
    setCaption( tools::translate( "InhabitantChangeConfinedStateDialog", "Change confine state" ) );
    resize( 320, 150 );
    // Main layout
    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this );
    pConfinedCheckBox_ = new QCheckBox( tools::translate( "InhabitantChangeConfinedStateDialog", "Confined" ), this );
    mainLayout->addWidget( pConfinedCheckBox_ );
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
// Name: InhabitantChangeConfinedStateDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeConfinedStateDialog::~InhabitantChangeConfinedStateDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeConfinedStateDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeConfinedStateDialog::Show()
{
    if( !selected_ )
        return;
    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeConfinedStateDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeConfinedStateDialog::Validate()
{
    if( ! selected_ )
        return;
    accept();

    actions::Action_ABC* action = actionsModel_.CreateInhabitantChangeConfinedStateAction( pConfinedCheckBox_->isChecked(), *selected_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeConfinedStateDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeConfinedStateDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeConfinedStateDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeConfinedStateDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeConfinedStateDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeConfinedStateDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantChangeConfinedStateDialog", "Change confine state" ), this, SLOT( Show() ) );
    }
}
