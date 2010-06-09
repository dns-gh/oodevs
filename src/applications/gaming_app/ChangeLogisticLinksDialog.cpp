// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeLogisticLinksDialog.h"
#include "moc_ChangeLogisticLinksDialog.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/UnitMagicAction.h"
#include "gaming/LogisticLinks.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/simulationsenders.h"
#include <qgrid.h>

using namespace actions;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Logistic links edition" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );
    QGrid* grid = new QGrid( 3, Qt::Horizontal, this );
    layout->addWidget( grid );
    grid->setSpacing( 5 );
    grid->setMargin( 5 );

    QLabel* color = new QLabel( grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( "yellow" ) );
    color->setMaximumWidth( 10 );
    QLabel* label = new QLabel( tr( "TC2" ), grid );
    label->setMaximumWidth( 150 );
    tc2Combo_ = new ValuedComboBox< const Automat_ABC* >( grid );
    tc2Combo_->setMinimumWidth( 200 );

    color = new QLabel( grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 128, 0, 0 ) );
    label = new QLabel( tr( "Maintenance superior" ), grid );
    maintenanceCombo_ = new ValuedComboBox< const Automat_ABC* >( grid );

    color = new QLabel( "", grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 255, 164, 200 ) );
    label = new QLabel( tr( "Medical superior" ), grid );
    medicalCombo_ = new ValuedComboBox< const Automat_ABC* >( grid );

    color = new QLabel( "  ", grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 255, 150, 10 ) );
    label = new QLabel( tr( "Supply superior" ), grid );
    supplyCombo_ = new ValuedComboBox< const Automat_ABC* >( grid );
   
    QHBox* box = new QHBox( this );
    layout->addWidget( box );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), box );
    okButton->setDefault( true );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    tc2Combo_->AddItem( tr( "None" ), 0 );
    maintenanceCombo_->AddItem( tr( "None" ), 0 );
    medicalCombo_->AddItem( tr( "None" ), 0 );
    supplyCombo_->AddItem( tr( "None" ), 0 );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog destructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::~ChangeLogisticLinksDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Show
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Show()
{
    if( !selected_ )
        return;
    const Automat_ABC& agent = *selected_;

    // $$$$ AGE 2006-08-24: display aussi ?
    const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
    if( !log )
        return;
    const AutomatType& type = agent.GetType();
    
    tc2Combo_->SetCurrentItem( log->GetTC2() );
    maintenanceCombo_->SetCurrentItem( log->GetMaintenance() );
    medicalCombo_->SetCurrentItem( log->GetMedical() );
    supplyCombo_->SetCurrentItem( log->GetSupply() );

    maintenanceCombo_->setEnabled( type.IsLogisticMaintenance() );
    medicalCombo_->setEnabled( type.IsLogisticMedical() );
    supplyCombo_->setEnabled( type.IsLogisticSupply() );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyCreated
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyCreated( const Automat_ABC& agent )
{
    // $$$$ AGE 2006-10-13: add only if agent is seen ?
    const AutomatType& type = agent.GetType();
    if( type.IsTC2() )
        tc2Combo_->AddItem( agent.GetName(), &agent );
    if( type.IsLogisticMaintenance() && !type.IsTC2() )
        maintenanceCombo_->AddItem( agent.GetName(), &agent );
    if( type.IsLogisticMedical() && !type.IsTC2() )
        medicalCombo_->AddItem( agent.GetName(), &agent );
    if( type.IsLogisticSupply() && !type.IsTC2() )
        supplyCombo_->AddItem( agent.GetName(), &agent );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyDeleted
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyDeleted( const Automat_ABC& agent )
{
    const AutomatType& type = agent.GetType();
    if( type.IsTC2() )
        tc2Combo_->RemoveItem( &agent );
    if( type.IsLogisticMaintenance() && !type.IsTC2() )
        maintenanceCombo_->RemoveItem( &agent );
    if( type.IsLogisticMedical() && !type.IsTC2() )
        medicalCombo_->RemoveItem( &agent );
    if( type.IsLogisticSupply() && !type.IsTC2() )
        supplyCombo_->RemoveItem( &agent );    
}

namespace
{
    unsigned int GetId( ValuedComboBox< const Automat_ABC* >& combo )
    {
        if( combo.isEnabled() )
        {
            const Automat_ABC* agent = combo.count() ? combo.GetValue() : 0;
            return agent ? agent->GetId() : 0;
        }
        else
            return ( unsigned int ) -1;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Validate()
{
    if( selected_ )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
        UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Identifier( it.NextElement(), GetId( *tc2Combo_ ) ) );
        action->AddParameter( *new parameters::Identifier( it.NextElement(), GetId( *maintenanceCombo_ ) ) );
        action->AddParameter( *new parameters::Identifier( it.NextElement(), GetId( *medicalCombo_ ) ) );
        action->AddParameter( *new parameters::Identifier( it.NextElement(), GetId( *supplyCombo_ ) ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->Attach( *new ActionTasker( selected_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
    Reject();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Reject()
{
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyContextMenu
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.Retrieve< LogisticLinks >() )
    {
        selected_ = &agent;
        menu.InsertItem( "Command", tr( "Change logistic links" ), this, SLOT( Show() ) );
    }
}
