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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"

#include <qgrid.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , publisher_( publisher )
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
    const Automat_ABC& agent = *selected_;

    // $$$$ AGE 2006-08-24: display aussi ?
    const LogisticLinks* log = static_cast< const LogisticLinks* >( agent.Retrieve< LogisticLinks_ABC >() );
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
    unsigned int SetId( ValuedComboBox< const Automat_ABC* >& combo, int& id )
    {
        if( combo.isEnabled() )
        {
            const Automat_ABC* agent = combo.count() ? combo.GetValue() : 0;
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
        ASN_MsgAutomatChangeLogisticLinks message;
        message().oid = selected_->GetId();

        message().m.oid_tc2Present            = SetId( *tc2Combo_, message().oid_tc2 );
        message().m.oid_maintenancePresent    = SetId( *maintenanceCombo_, message().oid_maintenance );
        message().m.oid_santePresent          = SetId( *medicalCombo_, message().oid_sante );
        message().m.oid_ravitaillementPresent = SetId( *supplyCombo_, message().oid_ravitaillement );
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
void ChangeLogisticLinksDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.Retrieve< LogisticLinks_ABC >() )
    {
        selected_ = &agent;
        menu.InsertItem( "Command", tr( "Change logistic links" ), this, SLOT( Show() ) );
    }
}
