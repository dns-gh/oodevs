//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ChangeHumanFactorsDialog.cpp $
// $Author: Nld $
// $Modtime: 3/02/05 17:04 $
// $Revision: 3 $
// $Workfile: MOS_ChangeHumanFactorsDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_ChangeHumanFactorsDialog.h"
#include "moc_MOS_ChangeHumanFactorsDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"

#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeHumanFactorsDialog::MOS_ChangeHumanFactorsDialog( QWidget* pParent  )
    : QDialog( pParent, "Changer facteurs humains" )
    , pAgent_( 0 )
{
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QGridLayout* pDataLayout = new QGridLayout( pMainLayout, 3, 2 );
    pDataLayout->setMargin( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( "Moral"     , this ), 0, 0 );
    pDataLayout->addWidget( new QLabel( "Experience", this ), 1, 0 );
    pDataLayout->addWidget( new QLabel( "Fatigue"   , this ), 2, 0 );

    pMoraleComboBox_     = new QComboBox( FALSE, this );
    pTirednessComboBox_  = new QComboBox( FALSE, this );
    pExperienceComboBox_ = new QComboBox( FALSE, this );

    pDataLayout->addWidget( pMoraleComboBox_    , 0, 1 );
    pDataLayout->addWidget( pExperienceComboBox_, 1, 1 );
    pDataLayout->addWidget( pTirednessComboBox_ , 2, 1 );

    for( uint i = 0; i < MOS_Morale::GetMorales().size(); ++i )
        pMoraleComboBox_->insertItem( "", i );
    for( MOS_Morale::CIT_MoraleMap itMorale = MOS_Morale::GetMorales().begin(); itMorale != MOS_Morale::GetMorales().end(); ++itMorale )
        pMoraleComboBox_->changeItem( itMorale->second->GetName().c_str(), itMorale->second->GetID() );

    for( i = 0; i < MOS_Tiredness::GetTirednesses().size(); ++i )
        pTirednessComboBox_->insertItem( "", i );
    for( MOS_Tiredness::CIT_TirednessMap itTiredness = MOS_Tiredness::GetTirednesses().begin(); itTiredness != MOS_Tiredness::GetTirednesses().end(); ++itTiredness )
        pTirednessComboBox_->changeItem( itTiredness->second->GetName().c_str(), itTiredness->second->GetID() );

    for( i = 0; i < MOS_Experience::GetExperiences().size(); ++i )
        pExperienceComboBox_->insertItem( "", i );
    for( MOS_Experience::CIT_ExperienceMap itExperience = MOS_Experience::GetExperiences().begin(); itExperience != MOS_Experience::GetExperiences().end(); ++itExperience )
        pExperienceComboBox_->changeItem( itExperience->second->GetName().c_str(), itExperience->second->GetID() );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );    
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeHumanFactorsDialog::~MOS_ChangeHumanFactorsDialog()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::SetAgent( const MOS_Agent& agent )
{
    assert( !pAgent_ );
    pAgent_ = &agent;

    pExperienceComboBox_->setCurrentItem( agent.GetExperience().GetID() );
    pTirednessComboBox_ ->setCurrentItem( agent.GetTiredness ().GetID() );
    pMoraleComboBox_    ->setCurrentItem( agent.GetMorale    ().GetID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::Validate()
{
    assert( pAgent_ );

    ASN1T_MagicActionChangeFacteursHumains msgData;
    msgData.m.experiencePresent = 1;
    msgData.m.fatiguePresent    = 1;
    msgData.m.moralPresent      = 1;
    
    msgData.experience = (ASN1T_EnumUnitExperience)pExperienceComboBox_->currentItem();
    msgData.fatigue    = (ASN1T_EnumUnitFatigue   )pTirednessComboBox_ ->currentItem();
    msgData.moral      = (ASN1T_EnumUnitMoral     )pMoraleComboBox_    ->currentItem();

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid                              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t                         = T_MsgUnitMagicAction_action_change_facteurs_humains;
    asnMsg.GetAsnMsg().action.u.change_facteurs_humains = &msgData;
    asnMsg.Send( 56 );

    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeHumanFactorsDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeHumanFactorsDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

