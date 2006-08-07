// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Attributes.h"
#include "astec_kernel/Controller.h"
#include "astec_kernel/Displayer_ABC.h"
#include "astec_kernel/Units.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_gui/GlTools_ABC.h"
#include "astec_kernel/DataDictionary.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Attributes constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Attributes::Attributes( Controller& controller, const CoordinateConverter_ABC& converter, DataDictionary& dictionary )
    : controller_( controller )
    , converter_ ( converter )
    , aggregated_( false ) 
{
    // $$$$ AGE 2006-06-22: unité !
    dictionary.Register( "Info/Etat opérationnel",               nRawOpState_ );
    dictionary.Register( "Info/Vitesse",                         nSpeed_ );
    dictionary.Register( "Info/Direction",                       nDirection_ );
    dictionary.Register( "Postures/Posture courante",            nCurrentPosture_ );
    dictionary.Register( "Communications/Brouillé",              bCommJammed_ );
    dictionary.Register( "Communications/Silence radio",         bRadioSilence_ );
    dictionary.Register( "Etat décisionnel/Etat opérationnel",   nOpState_ );
    dictionary.Register( "Etat décisionnel/RoE",                 nRulesOfEngagementState_ );
    dictionary.Register( "Etat décisionnel/Contact combat",      nCloseCombatState_ );
    dictionary.Register( "Etat décisionnel/Rapport de force",    nFightRateState_ );
    dictionary.Register( "Etat martial/Fait prisonnier",         bPrisoner_ );
    dictionary.Register( "Etat martial/Rendu",                   bSurrendered_ );
    dictionary.Register( "Etat martial/Réfugiés pris en compte", bRefugeesManaged_ );
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
        vPos_ = converter_.ConvertToXY( message.position );

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
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye );

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
    // $$$$ AGE 2006-03-17: Split attributes ?
    displayer.Group( "Info" )
                .Display( "Etat opérationnel:",                  nRawOpState_ * Units::percentage )
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

// -----------------------------------------------------------------------------
// Name: Attributes::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Attributes::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", nOpState_ );
    displayer.Display( "Rapport de force", nFightRateState_ );
    displayer.Display( "", nCloseCombatState_ );
    displayer.Display( "", nCurrentPosture_ );
    displayer.Display( "ROE", nRulesOfEngagementState_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Attributes::Draw( const Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( aggregated_ 
    || ! ( bDead_ || bRadioSilence_ || bRadarEnabled_ || bCommJammed_ )
    || ! viewport.IsInside( where ) )
        return;

    glPushAttrib( GL_CURRENT_BIT );
    glColor3f( 1, 1, 1 );
    if( bDead_ )
        tools.DrawIcon( xpm_skull, where, 150.f );
    if( bRadioSilence_ )
        tools.DrawIcon( xpm_talkie_interdit, where, 150.f );
    if( bRadarEnabled_ )
        tools.DrawIcon( xpm_radars_on, where, 150.f );
    if( bCommJammed_ )
        tools.DrawIcon( xpm_brouillage, where, 150.f );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Attributes::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Attributes::Aggregate( const bool& bDoMe )
{
    aggregated_ = bDoMe;
}

// -----------------------------------------------------------------------------
// Name: Attributes::ComputePostureFactor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
float Attributes::ComputePostureFactor( const std::vector< float >& factors ) const
{
    const float ratio = 0.01f * nPostureCompletionPourcentage_;
    return factors.at( nOldPosture_ ) * ( 1.f - ratio ) + factors.at( nCurrentPosture_ ) * ratio;
}
