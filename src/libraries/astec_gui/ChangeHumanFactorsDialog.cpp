// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ChangeHumanFactorsDialog.h"
#include "moc_ChangeHumanFactorsDialog.cpp"
#include "astec_gaming/ASN_Messages.h"
#include "astec_kernel/Controllers.h"
#include "astec_gaming/HumanFactors.h"
#include "astec_gaming/Tiredness.h"
#include "astec_gaming/Experience.h"
#include "astec_gaming/Morale.h"
#include "astec_kernel/Agent_ABC.h"

#include <QGrid.h>

namespace 
{
    template< typename Container, typename Combo >
    void Populate( const Container& cont, Combo& combo )
    {
        combo.Clear();
        for( typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
            combo.AddItem( it->second->GetName().c_str(), it->second->GetAsnID() );
    }
}
// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog constructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher )
    : QDialog( pParent, "Facteurs Humains" )
    , publisher_( publisher )
    , agent_( controllers )
{
    setCaption( "Facteurs humains" );
    QGridLayout* pLayout = new QGridLayout( this, 5, 2, 4 );
    pLayout->setRowStretch( 6, 1 );
    pLayout->setRowStretch( 2, 0 );
    
    pLayout->addWidget( new QLabel( tr( "Fatigue:" ), this ), 1, 0 );
    pTirednessCombo_ = new ValuedComboBox< ASN1T_EnumUnitFatigue >( this );
    pLayout->addWidget( pTirednessCombo_, 1, 1 );

    pLayout->addWidget( new QLabel( tr( "Moral:" ), this ), 2, 0 );
    pMoralCombo_ = new ValuedComboBox< ASN1T_EnumUnitMoral >( this );
    pLayout->addWidget( pMoralCombo_, 2, 1 );

    pLayout->addWidget( new QLabel( tr( "Experience:" ), this ), 3, 0 );
    pExperienceCombo_ = new ValuedComboBox< ASN1T_EnumUnitExperience >( this );
    pLayout->addWidget( pExperienceCombo_, 3, 1 );

    pAllUnitsCheckBox_ = new QCheckBox( tr( "Toutes les unit�s" ), this );
    pLayout->addMultiCellWidget( pAllUnitsCheckBox_, 4, 4, 0, 1 );

    QHBox* box = new QHBox( this );
    QPushButton* okBtn = new QPushButton( tr( "OK" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Annuler" ), box );
    okBtn->setDefault( true );
    pLayout->addMultiCellWidget( box, 5, 5, 0, 1, Qt::AlignCenter );

    connect( okBtn, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog destructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::~ChangeHumanFactorsDialog()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Show
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Show( const Agent_ABC& agent )
{
    Populate( Tiredness::GetTirednesses(), *pTirednessCombo_ );
    Populate( Morale::GetMorales(), *pMoralCombo_ );
    Populate( Experience::GetExperiences(), *pExperienceCombo_ );

    agent_ = &agent;
    
    if( const HumanFactors* humanFactors = agent_->Retrieve< HumanFactors >() )
    {
        pTirednessCombo_->SetCurrentItem( humanFactors->GetTiredness().GetAsnID() );
        pMoralCombo_->SetCurrentItem( humanFactors->GetMorale().GetAsnID() );
        pExperienceCombo_->SetCurrentItem( humanFactors->GetExperience().GetAsnID() );
        if ( agent_->GetAutomatType() )
            pAllUnitsCheckBox_->show();
        else
            pAllUnitsCheckBox_->hide();
        show();
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Validate
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Validate()
{
    if( ! agent_ )
        return;
    const ASN1T_EnumUnitFatigue tiredness = (ASN1T_EnumUnitFatigue)pTirednessCombo_->GetValue();
    const ASN1T_EnumUnitMoral moral = (ASN1T_EnumUnitMoral)pMoralCombo_->GetValue();
    const ASN1T_EnumUnitExperience experience = (ASN1T_EnumUnitExperience)pExperienceCombo_->GetValue();
    if ( pAllUnitsCheckBox_->isChecked() )
    {
        Iterator< const Agent_ABC& > it = agent_->CreateIterator();
        while( it.HasMoreElements() )
            SendMessage( it.NextElement().GetId(), tiredness, moral, experience );
    }
    SendMessage( agent_->GetId(), tiredness, moral, experience );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( uint id, ASN1T_EnumUnitFatigue tiredness, ASN1T_EnumUnitMoral moral, ASN1T_EnumUnitExperience experience )
{
    ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = id;

    ASN1T_MagicActionChangeFacteursHumains asnMagicAction;

    asnMsg.GetAsnMsg().action.t                         = T_MsgUnitMagicAction_action_change_facteurs_humains;
    asnMsg.GetAsnMsg().action.u.change_facteurs_humains = &asnMagicAction;

    asnMagicAction.m.fatiguePresent = asnMagicAction.m.moralPresent = asnMagicAction.m.experiencePresent = 1;
    asnMagicAction.fatigue    = tiredness;
    asnMagicAction.moral      = moral;
    asnMagicAction.experience = experience;
    asnMsg.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::sizeHint
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
QSize ChangeHumanFactorsDialog::sizeHint() const
{
    return QSize( 240, 120 );
}
