// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Attributes.h"
#include "App.h"
#include "World.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "Units.h"

// -----------------------------------------------------------------------------
// Name: Attributes constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Attributes::Attributes( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Attributes destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Attributes::~Attributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Attributes::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Attributes::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent )
    {
        MT_Vector2D vNewPos;
        // $$$$ AGE 2006-02-13: 
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.position.data, vNewPos );
        vPos_ = vNewPos;
    }

    if( message.m.etat_operationnel_brutPresent )
        nRawOpState_ = message.etat_operationnel_brut;

    if( message.m.etat_operationnelPresent )
        nOpState_ = (E_EtatOperationnel)message.etat_operationnel;

    if( message.m.disponibilite_au_tir_indirectPresent )
        nIndirectFireAvailability_  = (E_DisponibiliteAuTir)message.disponibilite_au_tir_indirect;
    
    if( message.m.posture_newPresent )
        nCurrentPosture_ = (E_UnitPosture)message.posture_new;

    if( message.m.posture_oldPresent )
        nOldPosture_ = (E_UnitPosture)message.posture_old;

    if( message.m.posture_pourcentagePresent )
        nPostureCompletionPourcentage_ = message.posture_pourcentage;

    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye ); // $$$$ AGE 2006-02-13: 

    if( message.m.mortPresent ) 
        bDead_ = message.mort;

    if( message.m.neutralisePresent )
        bNeutralized_ = message.neutralise;

    if( message.m.rapport_de_forcePresent )
        nFightRateState_ = (E_EtatRapFor)message.rapport_de_force;

    if( message.m.roePresent )
        nRulesOfEngagementState_ = (E_Roe)message.roe;

    if( message.m.roe_populationPresent )
        nRulesOfEngagementPopulationState_ = (E_RoePopulation)message.roe_population;

    if( message.m.combat_de_rencontrePresent )
        nCloseCombatState_ = (E_EtatCombatRencontre)message.combat_de_rencontre;

    if( message.m.embarquePresent )
        bLoadingState_ = message.embarque;

    if( message.m.transporteurs_disponiblesPresent )
        bHumanTransportersReady_ = message.transporteurs_disponibles;

    if( message.m.mode_furtif_actifPresent )
        bStealthModeEnabled_ = message.mode_furtif_actif;

    if( message.m.vitessePresent )
        nSpeed_ = message.vitesse;

    if( message.m.hauteurPresent )
        nAltitude_ = message.hauteur;

    if( message.m.directionPresent )
        nDirection_ = message.direction;

    if( message.m.communications_brouilleesPresent )
        bCommJammed_ = message.communications_brouillees;

    if( message.m.silence_radioPresent )
        bRadioSilence_ = message.silence_radio;

    if( message.m.radar_actifPresent  )
        bRadarEnabled_ = message.radar_actif;

    if( message.m.prisonnierPresent )
        bPrisoner_ = message.prisonnier;

    if( message.m.renduPresent )
        bSurrendered_ = message.rendu;

    if( message.m.refugie_pris_en_comptePresent )
        bRefugeesManaged_ = message.refugie_pris_en_compte;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Attributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Info" )
                .Display( "Etat Opérationnel:",                  nRawOpState_ * Units::percentage )
                .Display( "Mort:",                               bDead_ )
                .Display( "Neutralisé:",                         bNeutralized_ )
                .Display( "Vitesse:",                            nSpeed_ * Units::metersPerSecond )
                .Display( "Direction:",                          nDirection_ * Units::degrees )
                .Display( "Altitude:",                           nAltitude_  * Units::meters )
                .Display( "Troupes:",                            bLoadingState_ ? "Embarqué" : "Débarqué" )
                .Display( "Transporteurs d'hommes disponibles:", bHumanTransportersReady_ );

    displayer.Group( "Postures" )
                .Display( "Ancienne posture:", nOldPosture_ )
                .Item( "Nouvelle posture:" )
                    .Start( nCurrentPosture_ )
                    .Add( " (" )
                    .Add( nPostureCompletionPourcentage_ * Units::percentage )
                    .Add( ")" ).End();

    displayer.Group( "Communications" )
                .Display( "Brouillé:", bCommJammed_ )
                .Display( "Silence radio:", bRadioSilence_ );

    displayer.Group( "Etat décisionnel" )
                .Display( "Etat opérationnel:", nOpState_ )
                .Display( "RoE:", nRulesOfEngagementState_ )
                .Display( "RoE Population:", nRulesOfEngagementPopulationState_ )
                .Display( "Rapport de force:", nFightRateState_ )
                .Display( "Disponibilité au tir indirect:", nIndirectFireAvailability_ )
                .Display( "Contact combat:", nCloseCombatState_ );
  
    displayer.Group( "Etat martial" )
            .Display( "Fait prisonnier:", bPrisoner_ )
            .Display( "Rendu:", bSurrendered_ )
            .Display( "Réfugiés pris en compte:", bRefugeesManaged_ );
}
