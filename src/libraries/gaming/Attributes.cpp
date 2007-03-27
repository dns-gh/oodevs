// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Attributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "statusicons.h"
#include "Tools.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Attributes constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Attributes::Attributes( Controller& controller, const CoordinateConverter_ABC& converter, PropertiesDictionary& dictionary, const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : controller_( controller )
    , converter_ ( converter )
    , teamResolver_( teamResolver )
    , vPos_( 0, 0 )
    , nSpeed_( 0 )
    , nAltitude_( 0 )
    , nDirection_( 0 )
    , nRawOpState_( 0 )
    , nOpState_( (E_EtatOperationnel)0 )
    , nFightRateState_( (E_EtatRapFor)0 )
    , nRulesOfEngagementState_( (E_Roe)0 )
    , nRulesOfEngagementPopulationState_( (E_RoePopulation)0 )
    , nCloseCombatState_( (E_EtatCombatRencontre)0 )
    , bDead_( false )
    , bNeutralized_( false )
    , nOldPosture_( (E_UnitPosture)0 )
    , nCurrentPosture_( (E_UnitPosture)0 )
    , nPostureCompletionPourcentage_( 0 )
    , nInstallationState_( 0 )
    , nIndirectFireAvailability_( (E_DisponibiliteAuTir)0 )
    , bLoadingState_( false )
    , bHumanTransportersReady_( false )
    , bStealthModeEnabled_( false )
    , bRadioSilence_( false )
    , bCommJammed_( false )
    , bRadarEnabled_( false )
    , bPrisoner_( false )
    , surrenderedTo_( 0 )
    , bRefugeesManaged_( false )
    , aggregated_( false )
{
    CreateDictionary( dictionary );
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
// Name: Attributes::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Attributes::CreateDictionary( PropertiesDictionary& dictionary ) const
{
    // $$$$ AGE 2006-06-22: unité !
    dictionary.Register( *this, tools::translate( "Attributes", "Info/Operational state" ),               nRawOpState_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Info/Speed" ),                           nSpeed_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Info/Heading" ),                         nDirection_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Stances/Current stance" ),               nCurrentPosture_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Communications/Jammed" ),                bCommJammed_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Communications/Radio silence" ),         bRadioSilence_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Decisional state/Operational state" ),   nOpState_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Decisional state/Rules of engagement" ), nRulesOfEngagementState_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Decisional state/Intention" ),           nCloseCombatState_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Decisional state/Force ratio" ),         nFightRateState_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Military state/Prisoner" ),              bPrisoner_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Military state/Surrender" ),             surrenderedTo_ );
    dictionary.Register( *this, tools::translate( "Attributes", "Military state/Refugees picked up" ),    bRefugeesManaged_ );
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

    if( message.m.etat_installationPresent )
        nInstallationState_ = message.etat_installation;

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
        surrenderedTo_ = teamResolver_.Find( message.rendu );

    if( message.m.refugie_pris_en_comptePresent )
        bRefugeesManaged_ = message.refugie_pris_en_compte;

    controller_.Update( *(Attributes_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Attributes::Display( Displayer_ABC& displayer ) const
{
    // $$$$ AGE 2006-03-17: Split attributes ?
    displayer.Group( tools::translate( "Attributes", "Info" ) )
                .Display( tools::translate( "Attributes", "Operational state:" ),    nRawOpState_ * Units::percentage )
                .Display( tools::translate( "Attributes", "Dead:" ),                 bDead_ )
                .Display( tools::translate( "Attributes", "Neutralized:" ),          bNeutralized_ )
                .Display( tools::translate( "Attributes", "Speed:" ),                nSpeed_ * Units::kilometersPerHour )
                .Display( tools::translate( "Attributes", "Heading:" ),              nDirection_ * Units::degrees )
                .Display( tools::translate( "Attributes", "Height:" ),               nAltitude_  * Units::meters )
                .Display( tools::translate( "Attributes", "Troops:" ),               bLoadingState_ ? tools::translate( "Attributes", "on-board" ) : tools::translate( "Attributes", "off-board" ) )
                .Display( tools::translate( "Attributes", "Human transportation:" ), bHumanTransportersReady_ ? tools::translate( "Attributes", "Available" ) : tools::translate( "Attributes", "Unavailable" ) );

    displayer.Group( tools::translate( "Attributes", "Stances" ) )
                .Display( tools::translate( "Attributes", "Previous stance:" ), nOldPosture_ )
                .Item( tools::translate( "Attributes", "New stance:" ) )
                    .Start( nCurrentPosture_ )
                    .Add( " (" )
                    .Add( nPostureCompletionPourcentage_ * Units::percentage )
                    .Add( ")" ).End();
    displayer.Group( tools::translate( "Attributes", "Stances" ) )
                .Display( tools::translate( "Attributes", "Setup state:" ), nInstallationState_ * Units::percentage );
                

    displayer.Group( tools::translate( "Attributes", "Communications" ) )
                .Display( tools::translate( "Attributes", "Jammed:" ), bCommJammed_ )
                .Display( tools::translate( "Attributes", "Radio silence:" ), bRadioSilence_ );

    displayer.Group( tools::translate( "Attributes", "Decisional state" ) )
                .Display( tools::translate( "Attributes", "Operational state:" ), nOpState_ )
                .Display( tools::translate( "Attributes", "Rules of engagement:" ), nRulesOfEngagementState_ )
                .Display( tools::translate( "Attributes", "Population RoE:" ), nRulesOfEngagementPopulationState_ )
                .Display( tools::translate( "Attributes", "Force ratio:" ), nFightRateState_ )
                .Display( tools::translate( "Attributes", "Indirect fire availability:" ), nIndirectFireAvailability_ )
                .Display( tools::translate( "Attributes", "Intention:" ), nCloseCombatState_ );
  
    displayer.Group( tools::translate( "Attributes", "Military state" ) )
            .Display( tools::translate( "Attributes", "Prisoner:" ), bPrisoner_ )
            .Display( tools::translate( "Attributes", "Surrender:" ), surrenderedTo_ )
            .Display( tools::translate( "Attributes", "Refugees picked up:" ), bRefugeesManaged_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Attributes::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", nOpState_ );
    displayer.Display( tools::translate( "Attributes", "Force ratio:" ), nFightRateState_ );
    displayer.Display( "", nCloseCombatState_ );
    displayer.Display( "", nCurrentPosture_ );
    displayer.Display( tools::translate( "Attributes", "Rules of engagement:" ), nRulesOfEngagementState_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::DisplayInSummary
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void Attributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Attributes", "Speed:" ) , nSpeed_ * Units::kilometersPerHour )
             .Display( tools::translate( "Attributes", "Height:" ), nAltitude_ * Units::meters )
             .Display( tools::translate( "Attributes", "Troops:" ), bLoadingState_ ? tools::translate( "Attributes", "on-board" ) : tools::translate( "Attributes", "off-board" ) );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Attributes::Draw( const Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( aggregated_ 
    || ! ( bDead_ || bRadioSilence_ || bRadarEnabled_ || bCommJammed_ )
    || ! viewport.IsHotpointVisible() )
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
