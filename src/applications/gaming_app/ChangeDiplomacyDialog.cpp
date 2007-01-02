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
#include "gaming/Diplomacies.h"
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
    pMainLayout->setMargin( 5 );

    QHBox* box = new QHBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    new QLabel( tr( "Side" ), box );
    pArmy1ComboBox_ = new ValuedComboBox< const Team_ABC* >( box );
    new QLabel( tr( " is " ), box );

    pDiplomacyComboBox_ = new ValuedComboBox< ASN1T_EnumDiplomatie >( box );
    pDiplomacyComboBox_->AddItem( tr( "unknown" ), EnumDiplomatie::inconnu );
    pDiplomacyComboBox_->AddItem( tr( "friend" ),  EnumDiplomatie::ami );
    pDiplomacyComboBox_->AddItem( tr( "enemy" ),   EnumDiplomatie::ennemi );
    pDiplomacyComboBox_->AddItem( tr( "neutral" ), EnumDiplomatie::neutre );

    new QLabel( tr( "with side" ), box );
    pArmy2ComboBox_ = new ValuedComboBox< const Team_ABC* >( box );
    pMainLayout->addWidget( box );

    QLabel* spacer = new QLabel( this );
    spacer->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    pMainLayout->addWidget( spacer );

    box = new QHBox( this );
    box->setMaximumWidth( 200 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    pMainLayout->addWidget( box, 0, Qt::AlignCenter );

    connect( okBtn    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pArmy1ComboBox_, SIGNAL( activated( int ) ), SLOT( UpdateDiplomacy() ) );
    connect( pArmy2ComboBox_, SIGNAL( activated( int ) ), SLOT( UpdateDiplomacy() ) );

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

    asn.GetAsnMsg().oid_camp1  = pArmy1ComboBox_->GetValue()->GetId();
    asn.GetAsnMsg().oid_camp2  = pArmy2ComboBox_->GetValue()->GetId();
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
    pArmy1ComboBox_->AddItem( team.GetName(), &team );
    pArmy2ComboBox_->AddItem( team.GetName(), &team );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyDeleted( const Team_ABC& team )
{
    pArmy1ComboBox_->RemoveItem( &team );
    pArmy2ComboBox_->RemoveItem( &team );
}
    
// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::NotifyContextMenu
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::NotifyContextMenu( const Team_ABC& team, ContextMenu& menu )
{
    if( profile_.CanDoMagic( team ) )
    {
        pArmy1ComboBox_->SetCurrentItem( &team );
        UpdateDiplomacy();
        menu.InsertItem( "Command", tr( "Diplomacy" ), this, SLOT( show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::UpdateDiplomacy
// Created: SBO 2007-01-02
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::UpdateDiplomacy()
{
    const Team_ABC* army1 = pArmy1ComboBox_->GetValue();
    const Team_ABC* army2 = pArmy2ComboBox_->GetValue();
    if( army1 && army2 )
        pDiplomacyComboBox_->SetCurrentItem( army1->Get< Diplomacies >().GetRelationship( *army2 ) );
}
