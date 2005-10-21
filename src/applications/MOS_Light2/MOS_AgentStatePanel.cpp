// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentStatePanel.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:44 $
// $Revision: 14 $
// $Workfile: MOS_AgentStatePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AgentStatePanel.h"

#include "MOS_App.h"
#include "MOS_Tools.h"
#include "MOS_ReportListView.h"
#include "MOS_Agent.h"
#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentStatePanel constructor
/** @param  pParent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentStatePanel::MOS_AgentStatePanel( QWidget* pParent )
    : MOS_InfoPanel_ABC     ( pParent )
{
    QFont boldFont = this->font();
    boldFont.setBold( true );

    // Info groupbox
    QGroupBox* pInfoGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Info" ), this );

    new QLabel( tr( "Nom:" ), pInfoGroupBox );
    pNameLabel_ = new QLabel( pInfoGroupBox );
    pNameLabel_->setFont( boldFont );

    new QLabel( tr( "Etat Opérationnel:" ), pInfoGroupBox );
    pRawOpStateLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Mort:" ), pInfoGroupBox );
    pDeadLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Neutralisé:" ), pInfoGroupBox );
    pNeutralizedLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Vitesse:" ), pInfoGroupBox );
    pSpeedLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Direction:" ), pInfoGroupBox );
    pDirectionLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Altitude:" ), pInfoGroupBox );
    pAltitudeLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Troupes:" ), pInfoGroupBox );
    pBoardingStateLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Transporteurs d'hommes disponibles:" ), pInfoGroupBox );
    pHumanTransportersReadyLabel_ = new QLabel( pInfoGroupBox );

    // NBC groupbox
    QGroupBox* pNbcGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "NBC" ), this );

    new QLabel( tr( "Agents contaminants:" ), pNbcGroupBox );
    pNBCAgentsLabel_ = new QLabel( pNbcGroupBox );

    new QLabel( tr( "Contamination:" ), pNbcGroupBox );
    pContaminationLabel_ = new QLabel( pNbcGroupBox );

    // Stance groupbox
    QGroupBox* pStanceGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Postures" ), this );

    new QLabel( tr( "Ancienne posture:" ), pStanceGroupBox );
    pOldStanceLabel_ = new QLabel( pStanceGroupBox );

    new QLabel( tr( "Nouvelle posture:" ), pStanceGroupBox );
    pStanceLabel_ = new QLabel( pStanceGroupBox );

    // Comm groupbox
    QGroupBox* pCommGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Communications" ), this );

    new QLabel( tr( "Brouillé:" ), pCommGroupBox );
    pCommJammedLabel_ = new QLabel( pCommGroupBox );

    new QLabel( tr( "Silence radio:" ), pCommGroupBox );
    pRadioSilenceLabel_ = new QLabel( pCommGroupBox );

    // Human crap group box
    QGroupBox* pHumanFactorsBox = new QGroupBox( 2, Qt::Horizontal, tr( "Facteurs humains" ), this );

    new QLabel( tr( "Experience:" ), pHumanFactorsBox );
    pExperienceLabel_ = new QLabel( pHumanFactorsBox );

    new QLabel( tr( "Moral:" ), pHumanFactorsBox );
    pMoraleLabel_ = new QLabel( pHumanFactorsBox );

    new QLabel( tr( "Fatigue:" ), pHumanFactorsBox );
    pTirednessLabel_ = new QLabel( pHumanFactorsBox );

    // AI groupbox
    QGroupBox* pAIGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Etat décisionnel" ), this );

    new QLabel( tr( "Etat opérationnel:" ), pAIGroupBox );
    pOpStateLabel_ = new QLabel( pAIGroupBox );   

    new QLabel( tr( "RoE:" ), pAIGroupBox );
    pROELabel_ = new QLabel( pAIGroupBox );

    new QLabel( tr( "Rapport de force:" ), pAIGroupBox );
    pFightRateLabel_ = new QLabel( pAIGroupBox );

    new QLabel( tr( "Disponibilité au tir indirect:" ), pAIGroupBox );
    pIndirectFireAvailabilityPanel_ = new QLabel( pAIGroupBox );       

    new QLabel( tr( "Tenue NBC:" ), pAIGroupBox );
    pNBCSuitLabel_ = new QLabel( pAIGroupBox );

    new QLabel( tr( "Contact combat:" ), pAIGroupBox );
    pCloseCombatLabel_ = new QLabel( pAIGroupBox );
    pCloseCombatLabel_->setFont( boldFont );

    // Reinforcements
    QGroupBox* pGlobalReinforcementsGB = new QGroupBox( 2, Qt::Horizontal, tr( "Renforts" ), this );
    new QLabel( tr( "Renforce:" ), pGlobalReinforcementsGB );
    pReinforcedLabel_ = new QLabel( pGlobalReinforcementsGB );

    // Reinforcements groupbox (hidden when there are no reinforcements)
    pReinforcementsGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Renforts" ), this );
    pReinforcementsGroupBox_->hide();
    pReinforcementsLabel_ = new QLabel( pReinforcementsGroupBox_ );

    // Transport (hidden when there are no transported)
    pTransportedGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Pions Transportés" ), this );
    pTransportedGroupBox_->hide();
    pTransportedLabel_ = new QLabel( pTransportedGroupBox_ );

    // Logistic links (shown only for automates)
    pLogLinksGroupBox_ = new QGroupBox( 2, Qt::Horizontal, tr( "Liens logistiques" ), this );
    pLogLinksGroupBox_->hide();
    new QLabel( tr( "TC2:" ), pLogLinksGroupBox_ );
    pTC2_ = new QLabel( pLogLinksGroupBox_ );
    new QLabel( tr( "Supérieur maintenance:" ), pLogLinksGroupBox_ );
    pLogMaintenanceSuperior_ = new QLabel( pLogLinksGroupBox_ );
    new QLabel( tr( "Supérieur santé:" ), pLogLinksGroupBox_ );
    pLogMedicalSuperior_ = new QLabel( pLogLinksGroupBox_ );
    new QLabel( tr( "Supérieur ravitaillement:" ), pLogLinksGroupBox_ );
    pLogSupplySuperior_ = new QLabel( pLogLinksGroupBox_ );

    // Status
    QGroupBox* pWarStatus = new QGroupBox( 2, Qt::Horizontal, tr( "Etat martial" ), this ); // $$$$ AGE 2005-03-23: ?
    new QLabel( tr( "Fait prisonnier:" ), pWarStatus );
    pPrisonnerLabel_ = new QLabel( pWarStatus );
    new QLabel( tr( "Rendu:" ), pWarStatus );
    pSurrenderedLabel_ = new QLabel( pWarStatus );
    new QLabel( tr( "Réfugiés pris en compte:" ), pWarStatus );
    pRefugeeLabel_ = new QLabel( pWarStatus );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentStatePanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentStatePanel::~MOS_AgentStatePanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentStatePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentStatePanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentStatePanel::OnAgentUpdated
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_AgentStatePanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    if( agent.IsAutomate() )
    {
        if( agent.nTC2_ == 0 )
            pTC2_->setText( tr( "aucun" ) );
        else
            pTC2_->setText( QString::number( agent.nTC2_ ) );

        if( agent.nLogMaintenanceSuperior_ == 0 )
            pLogMaintenanceSuperior_->setText( tr( "aucun" ) );
        else
            pLogMaintenanceSuperior_->setText( QString::number( agent.nLogMaintenanceSuperior_ ) );

        if( agent.nLogMedicalSuperior_ == 0 )
            pLogMedicalSuperior_->setText( tr( "aucun" ) );
        else
            pLogMedicalSuperior_->setText( QString::number( agent.nLogMedicalSuperior_ ) );

        if( agent.nLogSupplySuperior_ == 0 )
            pLogSupplySuperior_->setText( tr( "aucun" ) );
        else
            pLogSupplySuperior_->setText( QString::number( agent.nLogSupplySuperior_ ) );
        pLogLinksGroupBox_->show();
    }

    pNameLabel_       ->setText( (agent.GetName()).c_str() );

    pRawOpStateLabel_ ->setText( QString( "%1 %" ).arg( agent.GetRawOpState() ) );

    pDeadLabel_       ->setText( agent.IsDead() ? tr( "Oui" ) : tr( "Non" ) );
    pNeutralizedLabel_->setText( agent.IsNeutralized() ? tr( "Oui" ) : tr( "Non" ) );

    pSpeedLabel_      ->setText( QString::number( agent.GetSpeed() ) );
    pDirectionLabel_  ->setText( QString::number( agent.GetDirection() ) + "°" );
    pAltitudeLabel_   ->setText( QString::number( agent.GetAltitude() ) + "m" );

    QString strStance = MOS_Tools::ToString( agent.GetStance() );
    strStance         = strStance + " (" + QString::number( agent.GetStanceCompletion() ) + "%)";
    pOldStanceLabel_  ->setText( MOS_Tools::ToString( agent.GetOldStance() ) );
    pStanceLabel_     ->setText( strStance );

    pROELabel_                     ->setText( ENT_Tr::ConvertFromReglesEngagement   ( agent.nRulesOfEngagementState_ ).c_str() );
    pCloseCombatLabel_             ->setText( ENT_Tr::ConvertFromEtatCombatRencontre( agent.nCloseCombatState_ ).c_str() );
    pFightRateLabel_               ->setText( ENT_Tr::ConvertFromEtatRapFor         ( agent.nFightRateState_ ).c_str() );
    pOpStateLabel_                 ->setText( ENT_Tr::ConvertFromEtatOperationnel   ( agent.nOpState_ ).c_str() );
    pIndirectFireAvailabilityPanel_->setText( ENT_Tr::ConvertFromDisponibiliteAuTir ( agent.nIndirectFireAvailability_ ).c_str() );
    
    uint nReinforced = agent.GetReinforced();
    if( nReinforced )
        pReinforcedLabel_->setText( QString( "[%1]" ).arg( nReinforced ) );
    else
        pReinforcedLabel_->setText( "-" );

    const MOS_Agent::T_AgentIdVector& reinforcements = agent.GetReinforcements();
    if( reinforcements.empty() )
        pReinforcementsGroupBox_->hide();
    else
    {
        pReinforcementsGroupBox_->show();
        std::stringstream strReinforcements;
        for( MOS_Agent::CIT_AgentIdVector itr = reinforcements.begin(); itr != reinforcements.end(); ++itr )
        {
            if( itr != reinforcements.begin() )
                strReinforcements << ", ";
            strReinforcements << "[" << *itr << "]";
        }
        pReinforcementsLabel_->setText( strReinforcements.str().c_str() );
    }

    const MOS_Agent::T_AgentIdVector& transported = agent.GetTransportees();
    if( transported.empty() )
        pTransportedGroupBox_->hide();
    else
    {
        pTransportedGroupBox_->show();
        std::stringstream strTransports;
        for( MOS_Agent::CIT_AgentIdVector itr = transported.begin(); itr != transported.end(); ++itr )
        {
            if( itr != transported.begin() )
                strTransports << ", ";
            strTransports << "[" << *itr << "]";
        }
        pTransportedLabel_->setText( strTransports.str().c_str() );
    }

    pPrisonnerLabel_->setText( agent.bPrisoner_ ? tr( "Oui" ) : tr( "Non" ) );
    pSurrenderedLabel_->setText( agent.bSurrendered_ ? tr( "Oui" ) : tr( "Non" ) );
    pRefugeeLabel_->setText( agent.bRefugeesManaged_ ? tr( "Oui" ) : tr( "Non" ) );

    pBoardingStateLabel_->setText( agent.IsLoaded() ? tr( "Embarqué" ) : tr( "Débarqué" ) );
    pHumanTransportersReadyLabel_->setText( agent.AreHumanTransportersReady() ? tr( "Oui" ) : tr( "Non" ) );

    pNBCSuitLabel_->setText( agent.IsNBSSuitOn() ? tr( "Mise" ) : tr( "Non mise" ) );
    pContaminationLabel_->setText( QString::number( agent.GetContamination() ) );
    
    pCommJammedLabel_->setText( agent.bCommJammed_ ? tr("Oui") : tr("Non") );
    pRadioSilenceLabel_->setText( agent.bRadioSilence_ ? tr("Oui") : tr("Non") );

    pExperienceLabel_->setText( tr( agent.GetExperience().GetName().c_str() ) );
    pMoraleLabel_->setText( tr( agent.GetMorale().GetName().c_str() ) );
    pTirednessLabel_->setText( tr( agent.GetTiredness().GetName().c_str() ) );

    const MOS_Agent::T_NbcAgentVector& contaminatingAgents = agent.GetContaminatingNBCAgents();
    if( ! contaminatingAgents.empty() )
    {
        std::stringstream strNBCAgents;
        for( MOS_Agent::CIT_NbcAgentVector it = contaminatingAgents.begin(); it != contaminatingAgents.end(); ++it )
        {
            if( it != contaminatingAgents.begin() )
                strNBCAgents << ", ";
            strNBCAgents << *it;
        }
        pNBCAgentsLabel_->setText( strNBCAgents.str().c_str() );
    }
    else
        pNBCAgentsLabel_->setText( tr( "Aucun" ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentStatePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_AgentStatePanel::OnClearSelection()
{
    pNameLabel_                  ->setText( "" );
    pRawOpStateLabel_            ->setText( "" );
    pDeadLabel_                  ->setText( "" );
    pNeutralizedLabel_           ->setText( "" );
    pSpeedLabel_                 ->setText( "" );
    pDirectionLabel_             ->setText( "" );
    pAltitudeLabel_              ->setText( "" );

    pOldStanceLabel_             ->setText( "" );
    pStanceLabel_                ->setText( "" );

    pROELabel_                   ->setText( "" );
    pCloseCombatLabel_           ->setText( "" );
    pFightRateLabel_             ->setText( "" );
    pOpStateLabel_               ->setText( "" );
    pIndirectFireAvailabilityPanel_->setText( "" );

    pReinforcedLabel_            ->setText( "" );
    pReinforcementsGroupBox_     ->hide();

    pTransportedGroupBox_        ->hide();

    pPrisonnerLabel_             ->setText( "" );
    pSurrenderedLabel_           ->setText( "" );
    pRefugeeLabel_               ->setText( "" );

    pBoardingStateLabel_         ->setText( "" );
    pHumanTransportersReadyLabel_->setText( "" );
    pNBCSuitLabel_               ->setText( "" );
    pContaminationLabel_         ->setText( "" );

    pCommJammedLabel_            ->setText( "" );
    pRadioSilenceLabel_          ->setText( "" );

    pExperienceLabel_            ->setText( "" );
    pMoraleLabel_                ->setText( "" );
    pTirednessLabel_             ->setText( "" );

    pNBCAgentsLabel_             ->setText( "" );

    pLogLinksGroupBox_           ->hide();
}
