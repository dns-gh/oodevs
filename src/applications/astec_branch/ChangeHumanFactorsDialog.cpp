// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ChangeHumanFactorsDialog.h"
#include "moc_ChangeHumanFactorsDialog.cpp"
#include "ASN_Messages.h"
#include "Controllers.h"
#include "HumanFactors.h"
#include "Tiredness.h"
#include "Experience.h"
#include "Morale.h"
#include "Agent.h"

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
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers )
    : QDialog( pParent, "Facteurs Humains" )
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

    pAllUnitsCheckBox_ = new QCheckBox( tr( "Toutes les unités" ), this );
    pLayout->addMultiCellWidget( pAllUnitsCheckBox_, 4, 4, 0, 1 );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    pButtonLayout->addWidget( pOKButton     );
    pButtonLayout->addWidget( pCancelButton );
    pOKButton->setDefault( true );
    pLayout->addMultiCellLayout( pButtonLayout, 5, 5, 0, 1, Qt::AlignCenter );

    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( hide() ) );
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
void ChangeHumanFactorsDialog::Show( const Agent& agent )
{
    Populate( Tiredness::GetTirednesses(), *pTirednessCombo_ );
    Populate( Morale::GetMorales(), *pMoralCombo_ );
    Populate( Experience::GetExperiences(), *pExperienceCombo_ );

    agent_ = &agent;
    const HumanFactors& humanFactors = agent_->Get< HumanFactors >();
    pTirednessCombo_->SetCurrentItem( humanFactors.GetTiredness().GetAsnID() );
    pMoralCombo_->SetCurrentItem( humanFactors.GetMorale().GetAsnID() );
    pExperienceCombo_->SetCurrentItem( humanFactors.GetExperience().GetAsnID() );
    if ( agent_->GetAutomatType() )
        pAllUnitsCheckBox_->show();
    else
        pAllUnitsCheckBox_->hide();
    show();
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
        Iterator< const Agent& > it = agent_->CreateIterator();
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
    asnMsg.Send();
};