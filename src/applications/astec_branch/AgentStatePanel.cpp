// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentStatePanel.h"
#include "Controller.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
#include "Agent.h"
#include "ActionController.h"
#include "Attributes.h"
#include "Contaminations.h"
#include "HumanFactors.h"
#include "Reinforcements.h"
#include "Tools.h"
#include "Experience.h"
#include "Morale.h"
#include "Tiredness.h"
#include "LogisticLinks.h"
#include "Transports.h"
#include "InfoPanel.h"

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( InfoPanel* info, Controller& controller, ActionController& actionController  )
    : InfoPanel_ABC( info, tr( "Etat" ) )
    , selected_( 0 )
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
                .AddItem( "Transporteurs d'hommes disponibles:" );
    display_->AddGroup( "NBC" )
                .AddItem( "Tenue NBC:" )
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

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::~AgentStatePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifySelected
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifySelected( const Agent* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            Show();
            NotifyUpdated( selected_->Get< Attributes >() );
            NotifyUpdated( selected_->Get< Contaminations >() );
            NotifyUpdated( selected_->Get< HumanFactors >() );
            NotifyUpdated( selected_->Get< Reinforcements >() );
            if( selected_->Retrieve< LogisticLinks >() )
                NotifyUpdated( selected_->Get< LogisticLinks >() );
            else
                display_->Group( "Liens logistiques" ).hide();
            NotifyUpdated( selected_->Get< Transports >() );
        }
        else
            Hide();
    };
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::ShouldUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentStatePanel::ShouldUpdate( const Extension& extension )
{
    return isVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Attributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    display_->Group( "Info" )
                .Display( "Nom:",                                selected_->GetName().c_str() )
                .Display( "Etat Opérationnel:",                  QString( "%1 %" ).arg( attributes.nRawOpState_ ) )
                .Display( "Mort:",                               Display::YesNo( attributes.bDead_ ) )
                .Display( "Neutralisé:",                         Display::YesNo( attributes.bNeutralized_ ) )
                .Display( "Vitesse:",                            QString::number( attributes.nSpeed_ ) )
                .Display( "Direction:",                          QString::number( attributes.nDirection_ ) + "°"  )
                .Display( "Altitude:",                           QString::number( attributes.nAltitude_ ) + "m" )
                .Display( "Troupes:",                            attributes.bLoadingState_ ? tr( "Embarqué" ) : tr( "Débarqué" ) )
                .Display( "Transporteurs d'hommes disponibles:", Display::YesNo( attributes.bHumanTransportersReady_ ) );

    QString strStance = Tools::ToString( attributes.nCurrentPosture_ )
                      + " (" + QString::number( attributes.nPostureCompletionPourcentage_ ) + "%)";
    display_->Group( "Postures" )
                .Display( "Ancienne posture:", Tools::ToString( attributes.nOldPosture_ ) )
                .Display( "Nouvelle posture:", strStance );

    display_->Group( "Communications" )
                .Display( "Brouillé:", Display::YesNo( attributes.bCommJammed_ ) )
                .Display( "Silence radio:", Display::YesNo( attributes.bRadioSilence_ ) );

    display_->Group( "Etat décisionnel" )
                .Display( "Etat opérationnel:", ENT_Tr::ConvertFromEtatOperationnel( attributes.nOpState_ ) )
                .Display( "RoE:", ENT_Tr::ConvertFromRoe( attributes.nRulesOfEngagementState_ ) )
                .Display( "RoE Population:", ENT_Tr::ConvertFromRoePopulation( attributes.nRulesOfEngagementPopulationState_ ) )
                .Display( "Rapport de force:", ENT_Tr::ConvertFromEtatRapFor( attributes.nFightRateState_ ) )
                .Display( "Disponibilité au tir indirect:", ENT_Tr::ConvertFromDisponibiliteAuTir ( attributes.nIndirectFireAvailability_ ) )
                .Display( "Contact combat:", ENT_Tr::ConvertFromEtatCombatRencontre( attributes.nCloseCombatState_ ) );

    display_->Group( "Etat martial" )
            .Display( "Fait prisonnier:", Display::YesNo( attributes.bPrisoner_ ) )
            .Display( "Rendu:", Display::YesNo( attributes.bSurrendered_ ) )
            .Display( "Réfugiés pris en compte:", Display::YesNo( attributes.bRefugeesManaged_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Contaminations& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    const std::vector< unsigned >& agents = attributes.contaminatingNbcAgents_;
    std::stringstream strNBCAgents;
    for( std::vector< unsigned >::const_iterator it = agents.begin(); it != agents.end(); ++it )
    {
        if( it != agents.begin() )
            strNBCAgents << ", ";
        strNBCAgents << *it; // $$$$ AGE 2006-02-16: resolve !
    }

    display_->Group( "NBC" )
                .Display( "Tenue NBC:", attributes.bNbcProtectionSuitWorn_ ? tr( "Mise" ) : tr( "Non mise" ) )
                .Display( "Agents contaminants:", agents.empty() ? tr( "Aucun" ) : strNBCAgents.str().c_str() )
                .Display( "Contamination:", QString::number( attributes.nContamination_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const HumanFactors& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    display_->Group( "Facteurs humains" )
                .Display( "Experience:", tr( attributes.pExperience_->GetName().c_str() ) )
                .Display( "Moral:", tr( attributes.pMorale_->GetName().c_str() ) )
                .Display( "Fatigue:", tr( attributes.pTiredness_->GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Reinforcements& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    display_->Group( "Renforts" )
        .Display( "Renforce:", attributes.reinforced_
                    ? QString( "[%1]" ).arg( attributes.reinforced_->GetId() )
                    : "-" );

    const std::vector< const Agent* >& agents = attributes.reinforcements_;
    if( agents.empty() )
        display_->Group( "Renforts " ).hide(); // $$$$ AGE 2006-02-16: ...
    else
    {
        std::stringstream strReinforcements;
        for( std::vector< const Agent* >::const_iterator it = agents.begin(); it != agents.end(); ++it ) {
            if( it != agents.begin() )
                strReinforcements << ", ";
            strReinforcements << "[" << (*it)->GetId() << "]";
        }
        display_->Group( "Renforts " ).Display( "", strReinforcements.str().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const LogisticLinks& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    display_->Group( "Liens logistiques" )
        // $$$$ AGE 2006-02-16: Plus malin que des ids ? a voir quand inversion de dépendance (genre liens, ...)
                .Display( "TC2:",                       Display::Id( a.GetTC2() ? a.GetTC2()->GetId() : 0 ) ) 
                .Display( "Supérieur maintenance:",     Display::Id( a.GetMaintenance() ? a.GetMaintenance()->GetId() : 0  ) )
                .Display( "Supérieur santé:",           Display::Id( a.GetMedical() ? a.GetMedical()->GetId() : 0 ) )
                .Display( "Supérieur ravitaillement:",  Display::Id( a.GetSupply() ? a.GetSupply()->GetId() : 0 ) );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Transports& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    const std::vector< const Agent* >& agents = attributes.transported_;
    if( agents.empty() )
       display_->Group( "Pions Transportés" ).hide();
    else
    {
        std::stringstream strTransports;
        for( std::vector< const Agent* >::const_iterator it = agents.begin(); it != agents.end(); ++it ) {
            if( it != agents.begin() )
                strTransports << ", ";
            strTransports << "[" << (*it)->GetId() << "]";
        }
        display_->Group( "Pions Transportés" ).Display( "", strTransports.str().c_str() );
    }
}

// $$$$ AGE 2006-02-16: Centraliser tout ca : 

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Agent& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyDeleted( const Agent& agent )
{
    if( selected_ = & agent )
        NotifySelected( 0 );
}
