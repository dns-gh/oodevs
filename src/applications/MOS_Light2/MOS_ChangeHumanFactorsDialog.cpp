// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_ChangeHumanFactorsDialog.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"
#include "MOS_Experience.h"
#include "MOS_Agent.h"
#include "MOS_ASN_Messages.h"
#include "moc_MOS_ChangeHumanFactorsDialog.cpp"

namespace 
{
    template< typename Container, typename Combo >
    void Populate( const Container& cont, Combo& combo )
    {
        for( typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
            combo.AddItem( it->second->GetName().c_str(), it->second->GetAsnID() );
    }
}
// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog constructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
MOS_ChangeHumanFactorsDialog::MOS_ChangeHumanFactorsDialog( QWidget* pParent )
    : QDialog( pParent, "Facteurs Humains" )
{
//    resize( 260, 100 );
    QGridLayout* pLayout = new QGridLayout( this, 4, 2, 4 );
    pLayout->setRowStretch( 5, 1 );
    pLayout->setRowStretch( 2, 0 );
    
    pLayout->addWidget( new QLabel( tr( "Fatigue:" ), this ), 1, 0, Qt::AlignLeft );
    pTirednessCombo_ = new MT_ValuedComboBox< ASN1T_EnumUnitFatigue >( this );
    pLayout->addWidget( pTirednessCombo_, 1, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Moral:" ), this ), 2, 0, Qt::AlignLeft );
    pMoralCombo_ = new MT_ValuedComboBox< ASN1T_EnumUnitMoral >( this );
    pLayout->addWidget( pMoralCombo_, 2, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Experience:" ), this ), 3, 0, Qt::AlignLeft );
    pExperienceCombo_ = new MT_ValuedComboBox< ASN1T_EnumUnitExperience >( this );
    pLayout->addWidget( pExperienceCombo_, 3, 1, Qt::AlignRight );

    Populate( MOS_Tiredness::GetTirednesses(), *pTirednessCombo_ );
    Populate( MOS_Morale::GetMorales(), *pMoralCombo_ );
    Populate( MOS_Experience::GetExperiences(), *pExperienceCombo_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    pButtonLayout->addWidget( pOKButton     );
    pButtonLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );
    pLayout->addMultiCellLayout( pButtonLayout, 4, 4, 0, 1, Qt::AlignCenter );

    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog destructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
MOS_ChangeHumanFactorsDialog::~MOS_ChangeHumanFactorsDialog()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::SetAgent
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::SetAgent( const MOS_Agent& agent )
{
    pAgent_ = &agent;
    pTirednessCombo_->SetCurrentItem( agent.GetTiredness().GetAsnID() );
    pMoralCombo_->SetCurrentItem( agent.GetMorale().GetAsnID() );
    pExperienceCombo_->SetCurrentItem( agent.GetExperience().GetAsnID() );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::Validate
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::Validate()
{
    const ASN1T_EnumUnitFatigue    fatigue    = (ASN1T_EnumUnitFatigue)   pTirednessCombo_ ->GetValue();
    const ASN1T_EnumUnitMoral      moral      = (ASN1T_EnumUnitMoral)     pMoralCombo_     ->GetValue();
    const ASN1T_EnumUnitExperience experience = (ASN1T_EnumUnitExperience)pExperienceCombo_->GetValue();
    if( ! pAgent_ )
        RUNTIME_ERROR;
    SendMessage( pAgent_->GetAgentID(), fatigue, moral, experience );
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::SendMessage( uint id, ASN1T_EnumUnitFatigue fatigue, ASN1T_EnumUnitMoral moral, ASN1T_EnumUnitExperience experience )
{
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = id;

    ASN1T_MagicActionChangeFacteursHumains asnMagicAction;

    asnMsg.GetAsnMsg().action.t                         = T_MsgUnitMagicAction_action_change_facteurs_humains;
    asnMsg.GetAsnMsg().action.u.change_facteurs_humains = &asnMagicAction;

    asnMagicAction.m.fatiguePresent = asnMagicAction.m.moralPresent = asnMagicAction.m.experiencePresent = 1;
    asnMagicAction.fatigue    = fatigue;
    asnMagicAction.moral      = moral;
    asnMagicAction.experience = experience;
    asnMsg.Send( 42 );
};