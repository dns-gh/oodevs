// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"

#include "ChangeDiplomacyDialog.h"
#include "moc_ChangeDiplomacyDialog.cpp"
#include "astec_kernel/Controllers.h"

#include "astec_gaming/ASN_Messages.h"
#include "astec_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher )
    : QDialog( parent )
    , controllers_( controllers )
    , publisher_( publisher )
{
    setCaption( tr( "Diplomatie" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QHBoxLayout* pDataLayout = new QHBoxLayout( pMainLayout );
    pDataLayout->setMargin( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( tr( "L'armée" ), this ) );

    pArmy1ComboBox_ = new ValuedComboBox< unsigned long >( this );
    pDataLayout->addWidget( pArmy1ComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( " est " ) , this ) );

    pDiplomacyComboBox_ = new ValuedComboBox<ASN1T_EnumDiplomatie>( this );
    pDiplomacyComboBox_->AddItem( tr( "inconnue" ), EnumDiplomatie::inconnu );
    pDiplomacyComboBox_->AddItem( tr( "amie" ),     EnumDiplomatie::ami );
    pDiplomacyComboBox_->AddItem( tr( "ennemie" ),  EnumDiplomatie::ennemi );
    pDiplomacyComboBox_->AddItem( tr( "neutre" ),   EnumDiplomatie::neutre );
    pDataLayout->addWidget( pDiplomacyComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( "avec l'armée" ), this ) );

    pArmy2ComboBox_ = new ValuedComboBox< unsigned long >( this );
    pDataLayout->addWidget( pArmy2ComboBox_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

    controllers_.Register( *this );

    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::~ChangeDiplomacyDialog()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Validate
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Validate()
{
    ASN_MsgChangeDiplomatie asn;

    asn.GetAsnMsg().oid_camp1  = pArmy1ComboBox_->GetValue();
    asn.GetAsnMsg().oid_camp2  = pArmy2ComboBox_->GetValue();
    asn.GetAsnMsg().diplomatie = pDiplomacyComboBox_->GetValue();
    asn.Send( publisher_ );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Reject
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyCreated( const Team_ABC& team )
{
    pArmy1ComboBox_->AddItem( team.GetName().c_str(), team.GetId() );
    pArmy2ComboBox_->AddItem( team.GetName().c_str(), team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyDeleted( const Team_ABC& team )
{
    pArmy1ComboBox_->RemoveItem( team.GetId() );
    pArmy2ComboBox_->RemoveItem( team.GetId() );
}
    
// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyContextMenu
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyContextMenu( const Team_ABC& team, ContextMenu& menu )
{
    pArmy1ComboBox_->SetCurrentItem( team.GetId() );
    menu.InsertItem( "Commande", tr( "Diplomatie" ), this, SLOT( show() ) );
}
