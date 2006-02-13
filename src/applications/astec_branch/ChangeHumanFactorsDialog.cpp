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
#include "Tiredness.h"
#include "Morale.h"
#include "Experience.h"
#include "Agent.h"
#include "ASN_Messages.h"
#include "moc_ChangeHumanFactorsDialog.cpp"

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
// Name: ChangeHumanFactorsDialog constructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent )
    : QDialog( pParent, "Facteurs Humains" )
{
//    resize( 260, 100 );
    QGridLayout* pLayout = new QGridLayout( this, 5, 2, 4 );
    pLayout->setRowStretch( 6, 1 );
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

    pAllUnitsCheckBox_ = new QCheckBox( tr( "Toute les unitées" ), this );
    pLayout->addWidget( pAllUnitsCheckBox_, 4, 0, Qt::AlignLeft );

    Populate( Tiredness::GetTirednesses(), *pTirednessCombo_ );
    Populate( Morale::GetMorales(), *pMoralCombo_ );
    Populate( Experience::GetExperiences(), *pExperienceCombo_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    pButtonLayout->addWidget( pOKButton     );
    pButtonLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );
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
// Name: ChangeHumanFactorsDialog::SetAgent
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SetAgent( const Agent& agent )
{
    pAgent_ = &agent;
    pTirednessCombo_->SetCurrentItem( agent.GetTiredness().GetAsnID() );
    pMoralCombo_->SetCurrentItem( agent.GetMorale().GetAsnID() );
    pExperienceCombo_->SetCurrentItem( agent.GetExperience().GetAsnID() );
    if ( pAgent_->IsAutomate() )
        pAllUnitsCheckBox_->show();
    else
        pAllUnitsCheckBox_->hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Validate
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Validate()
{
    const ASN1T_EnumUnitFatigue    fatigue    = (ASN1T_EnumUnitFatigue)   pTirednessCombo_ ->GetValue();
    const ASN1T_EnumUnitMoral      moral      = (ASN1T_EnumUnitMoral)     pMoralCombo_     ->GetValue();
    const ASN1T_EnumUnitExperience experience = (ASN1T_EnumUnitExperience)pExperienceCombo_->GetValue();
    if( ! pAgent_ )
        RUNTIME_ERROR;
    if ( pAgent_->IsAutomate() && pAllUnitsCheckBox_->isChecked() )
    {
        const Agent::T_AgentVector& children = const_cast< Agent* >(pAgent_)->GetChildren();       
        for( Agent::CIT_AgentVector it = children.begin(); it != children.end(); ++it )
            SendMessage( (*it)->GetID(), fatigue, moral, experience );
    }
    SendMessage( pAgent_->GetID(), fatigue, moral, experience );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( uint id, ASN1T_EnumUnitFatigue fatigue, ASN1T_EnumUnitMoral moral, ASN1T_EnumUnitExperience experience )
{
    ASN_MsgUnitMagicAction asnMsg;
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