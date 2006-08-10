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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentStatePanel.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:44 $
// $Revision: 14 $
// $Workfile: AgentStatePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentStatePanel.h"

#include "App.h"
#include "Tools.h"
#include "ReportListView.h"
#include "Agent.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Morale.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
/** @param  pParent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( QWidget* pParent )
    : InfoPanel_ABC( pParent )
    , display_( 0 )
{
    display_ = new Display( this );
    display_->AddGroup( "Info" )
                .AddItem( "Nom:", true )
                .AddItem( "Etat Opérationnel:" )
                .AddItem( "Mort:" )
                .AddItem( "Neutralisé:" )
                .AddItem( "Vitesse:" )
                .AddItem( "Direction:" )
                .AddItem( "Altitude:" )
                .AddItem( "Troupes:" )
                .AddItem( "Transporteurs d'hommes disponibles:" )
                .AddItem( "Etat installation:" );
        
    display_->AddGroup( "NBC" )
                .AddItem( "Agents contaminants:" )
                .AddItem( "Contamination:" );
    display_->AddGroup( "Postures" )
                .AddItem( "Ancienne posture:" )
                .AddItem( "Nouvelle posture:" );
    display_->AddGroup( "Communications" )
                .AddItem( "Brouillé:" )
                .AddItem( "Silence radio:" );
    display_->AddGroup( "Facteurs humains" )
                .AddItem( "Experience:" )
                .AddItem( "Moral:" )
                .AddItem( "Fatigue:" );
    display_->AddGroup( "Etat décisionnel" )
                .AddItem( "Etat opérationnel:" )
                .AddItem( "RoE:" )
                .AddItem( "RoE Population:" )
                .AddItem( "Rapport de force:" )
                .AddItem( "Disponibilité au tir indirect:" )
                .AddItem( "Tenue NBC:" ) // $$$$ AGE 2006-02-09: Déplacer dans nbc ?
                .AddItem( "Contact combat:" );
    display_->AddGroup( "Renforts" )
                .AddItem( "Renforce:" );

    // $$$$ AGE 2006-02-09: Fusses
    display_->AddGroup( "Renforts " )
                .AddItem( "" );
    display_->AddGroup( "Pions Transportés" )
                .AddItem( "" );

    display_->AddGroup( "Liens logistiques" )
                .AddItem( "TC2:" )
                .AddItem( "Supérieur maintenance:" )
                .AddItem( "Supérieur santé:" )
                .AddItem( "Supérieur ravitaillement:" );

    display_->AddGroup( "Etat martial" )
                .AddItem( "Fait prisonnier:" )
                .AddItem( "Rendu:" )
                .AddItem( "Réfugiés pris en compte:" );

    // Reinforcements groupbox (hidden when there are no reinforcements)
//    pReinforcementsGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Renforts" ), this );
//    pReinforcementsGroupBox_->hide();
//    pReinforcementsLabel_ = new QLabel( pReinforcementsGroupBox_ );

    // Transport (hidden when there are no transported)
//    pTransportedGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Pions Transportés" ), this );
//    pTransportedGroupBox_->hide();
//    pTransportedLabel_ = new QLabel( pTransportedGroupBox_ );

    // Logistic links (shown only for automates)
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentStatePanel::~AgentStatePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void AgentStatePanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::OnAgentUpdated
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void AgentStatePanel::OnAgentUpdated( Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    if( agent.IsAutomate() )
        display_->Group( "Liens logistiques" )
                    .Display( "TC2:",                       Display::Id( agent.nTC2_  ) ) 
                    .Display( "Supérieur maintenance:",     Display::Id( agent.nLogMaintenanceSuperior_  ) )
                    .Display( "Supérieur santé:",           Display::Id( agent.nLogMedicalSuperior_ ) )
                    .Display( "Supérieur ravitaillement:",  Display::Id( agent.nLogSupplySuperior_ ) );
    else 
        display_->Group( "Liens logistiques" ).hide();

    display_->Group( "Info" )
                .Display( "Nom:",               (agent.GetName()).c_str() )
                .Display( "Etat Opérationnel:", QString( "%1 %" ).arg( agent.GetRawOpState() ) )
                .Display( "Mort:",       Display::YesNo( agent.IsDead() ) )
                .Display( "Neutralisé:", Display::YesNo( agent.IsNeutralized() ) )
                .Display( "Vitesse:", QString::number( agent.GetSpeed() ) )
                .Display( "Direction:", QString::number( agent.GetDirection() ) + "°"  )
                .Display( "Altitude:", QString::number( agent.GetAltitude() ) + "m" )
                .Display( "Troupes:", agent.IsLoaded() ? tr( "Embarqué" ) : tr( "Débarqué" ) )
                .Display( "Transporteurs d'hommes disponibles:", Display::YesNo( agent.AreHumanTransportersReady() ) )
                .Display( "Etat installation:", QString( "%1 %" ).arg( agent.nInstallationState_ ) );

    const Agent::T_NbcAgentVector& contaminatingAgents = agent.GetContaminatingNBCAgents();
    std::stringstream strNBCAgents;
    for( Agent::CIT_NbcAgentVector it = contaminatingAgents.begin(); it != contaminatingAgents.end(); ++it )
    {
        if( it != contaminatingAgents.begin() )
            strNBCAgents << ", ";
        strNBCAgents << *it;
    }
    display_->Group( "NBC" )
                .Display( "Agents contaminants:", contaminatingAgents.empty() ? tr( "Aucun" ) : strNBCAgents.str().c_str() )
                .Display( "Contamination:", QString::number( agent.GetContamination() ) );

    QString strStance = Tools::ToString( agent.GetStance() );
    strStance         = strStance + " (" + QString::number( agent.GetStanceCompletion() ) + "%)";
    display_->Group( "Postures" )
                .Display( "Ancienne posture:", Tools::ToString( agent.GetOldStance() ) )
                .Display( "Nouvelle posture:", strStance );

    display_->Group( "Communications" )
                .Display( "Brouillé:", Display::YesNo( agent.bCommJammed_ ) )
                .Display( "Silence radio:", Display::YesNo( agent.bRadioSilence_ ) );
    
    display_->Group( "Facteurs humains" )
                .Display( "Experience:", tr( agent.GetExperience().GetName().c_str() ) )
                .Display( "Moral:", tr( agent.GetMorale().GetName().c_str() ) )
                .Display( "Fatigue:", tr( agent.GetTiredness().GetName().c_str() ) );

    display_->Group( "Etat décisionnel" )
                .Display( "Etat opérationnel:", ENT_Tr::ConvertFromEtatOperationnel( agent.nOpState_ ) )
                .Display( "RoE:", ENT_Tr::ConvertFromRoe( agent.nRulesOfEngagementState_ ) )
                .Display( "RoE Population:", ENT_Tr::ConvertFromRoePopulation( agent.nRulesOfEngagementPopulationState_ ) )
                .Display( "Rapport de force:", ENT_Tr::ConvertFromEtatRapFor( agent.nFightRateState_ ) )
                .Display( "Disponibilité au tir indirect:", ENT_Tr::ConvertFromDisponibiliteAuTir ( agent.nIndirectFireAvailability_ ) )
                .Display( "Tenue NBC:", agent.IsNBSSuitOn() ? tr( "Mise" ) : tr( "Non mise" ) )
                .Display( "Contact combat:", ENT_Tr::ConvertFromEtatCombatRencontre( agent.nCloseCombatState_ ) );

    display_->Group( "Renforts" )
        .Display( "Renforce:", agent.GetReinforced() ? QString( "[%1]" ).arg( agent.GetReinforced() ) : "-" );

    const Agent::T_AgentIdVector& reinforcements = agent.GetReinforcements();
    if( reinforcements.empty() )
        display_->Group( "Renforts " ).hide();
    else
    {
        std::stringstream strReinforcements;
        for( Agent::CIT_AgentIdVector itr = reinforcements.begin(); itr != reinforcements.end(); ++itr ) {
            if( itr != reinforcements.begin() )
                strReinforcements << ", ";
            strReinforcements << "[" << *itr << "]";
        }
        display_->Group( "Renforts " ).Display( "", strReinforcements.str().c_str() );
    }

    const Agent::T_AgentIdVector& transported = agent.GetTransportees();
    if( transported.empty() )
       display_->Group( "Pions Transportés" ).hide();
    else
    {
        std::stringstream strTransports;
        for( Agent::CIT_AgentIdVector itr = transported.begin(); itr != transported.end(); ++itr ) {
            if( itr != transported.begin() )
                strTransports << ", ";
            strTransports << "[" << *itr << "]";
        }
        display_->Group( "Pions Transportés" ).Display( "", strTransports.str().c_str() );
    }

    display_->Group( "Etat martial" )
            .Display( "Fait prisonnier:", Display::YesNo( agent.bPrisoner_ ) )
            .Display( "Rendu:", Display::YesNo( agent.bSurrendered_ ) )
            .Display( "Réfugiés pris en compte:", Display::YesNo( agent.bRefugeesManaged_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void AgentStatePanel::OnClearSelection()
{
    display_->Clear();
    display_->Group( "Renforts " ).hide();
    display_->Group( "Pions Transportés" ).hide();
    display_->Group( "Liens logistiques" ).hide();
}
