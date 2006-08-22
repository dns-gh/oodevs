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
#include "gaming/ASN_Messages.h"
#include "gaming/LogisticLinks.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"

#include <qgrid.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher )
    : QDialog( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , selected_( controllers )
{
    setCaption( tr( "Changement des liens logisitiques" ) );
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
    tc2Combo_ = new ValuedComboBox< const Agent_ABC* >( grid );
    tc2Combo_->setMinimumWidth( 200 );

    color = new QLabel( grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 128, 0, 0 ) );
    label = new QLabel( tr( "Superieur maintenance" ), grid );
    maintenanceCombo_ = new ValuedComboBox< const Agent_ABC* >( grid );

    color = new QLabel( "", grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 255, 164, 200 ) );
    label = new QLabel( tr( "Superieur santé" ), grid );
    medicalCombo_ = new ValuedComboBox< const Agent_ABC* >( grid );

    color = new QLabel( "  ", grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( 255, 150, 10 ) );
    label = new QLabel( tr( "Superieur ravitaillement" ), grid );
    supplyCombo_ = new ValuedComboBox< const Agent_ABC* >( grid );
   
    QHBox* box = new QHBox( this );
    layout->addWidget( box );
    QPushButton* okButton = new QPushButton( tr( "OK" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Annuler" ), box );
    okButton->setDefault( true );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    tc2Combo_->AddItem( tr( "Aucun" ), 0 );
    maintenanceCombo_->AddItem( tr( "Aucun" ), 0 );
    medicalCombo_->AddItem( tr( "Aucun" ), 0 );
    supplyCombo_->AddItem( tr( "Aucun" ), 0 );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog destructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::~ChangeLogisticLinksDialog()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Show
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Show()
{
    if( !selected_ )
        return;
    const Agent_ABC& agent = *selected_;

    const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
    const AutomatType* type = agent.GetAutomatType();
    if( !log || !type )
        return;
    tc2Combo_->SetCurrentItem( log->GetTC2() );
    maintenanceCombo_->SetCurrentItem( log->GetMaintenance() );
    medicalCombo_->SetCurrentItem( log->GetMedical() );
    supplyCombo_->SetCurrentItem( log->GetSupply() );

    maintenanceCombo_->setEnabled( type->IsTC2() || type->IsLogisticMaintenance() );
    medicalCombo_->setEnabled( type->IsTC2() || type->IsLogisticMedical() );
    supplyCombo_->setEnabled( type->IsTC2() || type->IsLogisticSupply() );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyCreated
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyCreated( const Agent_ABC& agent )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type )
        return;
    if( type->IsTC2() )
        tc2Combo_->AddItem( agent.GetName().c_str(), &agent );
    if( type->IsLogisticMaintenance() )
        maintenanceCombo_->AddItem( agent.GetName().c_str(), &agent );
    if( type->IsLogisticMedical() )
        medicalCombo_->AddItem( agent.GetName().c_str(), &agent );
    if( type->IsLogisticSupply() )
        supplyCombo_->AddItem( agent.GetName().c_str(), &agent );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyDeleted
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyDeleted( const Agent_ABC& agent )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type )
        return;
    if( type->IsTC2() )
        tc2Combo_->RemoveItem( &agent );
    if( type->IsLogisticMaintenance() )
        maintenanceCombo_->RemoveItem( &agent );
    if( type->IsLogisticMedical() )
        medicalCombo_->RemoveItem( &agent );
    if( type->IsLogisticSupply() )
        supplyCombo_->RemoveItem( &agent );    
}

namespace
{
    unsigned int SetId( ValuedComboBox< const Agent_ABC* >& combo, unsigned int& id )
    {
        if( combo.isEnabled() )
        {
            const Agent_ABC* agent = combo.GetValue();
            id = agent ? agent->GetId() : 0;
        }
        return combo.isEnabled();
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
        ASN_MsgChangeLiensLogistiques message;
        message.GetAsnMsg().oid_automate = selected_->GetId();

        message.GetAsnMsg().m.oid_tc2Present = SetId( *tc2Combo_, message.GetAsnMsg().oid_tc2 );
        message.GetAsnMsg().m.oid_maintenancePresent = SetId( *maintenanceCombo_, message.GetAsnMsg().oid_maintenance );
        message.GetAsnMsg().m.oid_santePresent = SetId( *medicalCombo_, message.GetAsnMsg().oid_sante );
        message.GetAsnMsg().m.oid_ravitaillementPresent = SetId( *supplyCombo_, message.GetAsnMsg().oid_ravitaillement );
	    message.Send( publisher_ );
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
void ChangeLogisticLinksDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type || !agent.Retrieve< LogisticLinks >() )
        return;
    selected_ = &agent;
    menu.InsertItem( "Commande", tr( "Changer les liens logistiques" ), this, SLOT( Show() ) );
}
