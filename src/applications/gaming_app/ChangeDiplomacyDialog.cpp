// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeDiplomacyDialog.h"
#include "moc_ChangeDiplomacyDialog.cpp"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , profile_( profile )
{
    setCaption( tr( "Diplomacy" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QHBoxLayout* pDataLayout = new QHBoxLayout( pMainLayout );
    pDataLayout->setMargin( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( tr( "Side" ), this ) );

    pArmy1ComboBox_ = new ValuedComboBox< unsigned long >( this );
    pDataLayout->addWidget( pArmy1ComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( " is " ) , this ) );

    pDiplomacyComboBox_ = new ValuedComboBox<ASN1T_EnumDiplomatie>( this );
    pDiplomacyComboBox_->AddItem( tr( "unknown" ), EnumDiplomatie::inconnu );
    pDiplomacyComboBox_->AddItem( tr( "friend" ),  EnumDiplomatie::ami );
    pDiplomacyComboBox_->AddItem( tr( "enemy" ),   EnumDiplomatie::ennemi );
    pDiplomacyComboBox_->AddItem( tr( "neutral" ), EnumDiplomatie::neutre );
    pDataLayout->addWidget( pDiplomacyComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( "with side" ), this ) );

    pArmy2ComboBox_ = new ValuedComboBox< unsigned long >( this );
    pDataLayout->addWidget( pArmy2ComboBox_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pOKButton     = new QPushButton( tr("Ok")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
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
    pArmy1ComboBox_->AddItem( team.GetName(), team.GetId() );
    pArmy2ComboBox_->AddItem( team.GetName(), team.GetId() );
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
    if( profile_.CanDoMagic( team ) )
    {
        pArmy1ComboBox_->SetCurrentItem( team.GetId() );
        menu.InsertItem( "Commande", tr( "Diplomacy" ), this, SLOT( show() ) );
    }
}
