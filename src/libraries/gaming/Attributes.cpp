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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Tools.h"
#include "statusicons.h"
#include "Tools.h"
#include <boost/foreach.hpp>
#include <set>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Attributes constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Attributes::Attributes( kernel::Entity_ABC& entity, Controller& controller, const CoordinateConverter_ABC& converter,
                        PropertiesDictionary& dictionary, const tools::Resolver_ABC< Team_ABC >& teamResolver )
    : entity_                           ( entity )
    , controller_                       ( controller )
    , converter_                        ( converter )
    , teamResolver_                     ( teamResolver )
    , vPos_                             ( 0, 0 )
    , nSpeed_                           ( 0 )
    , nAltitude_                        ( 0 )
    , nDirection_                       ( 0 )
    , nRawOpState_                      ( 0 )
    , nOpState_                         ( eOperationalStatus_Operationnel )
    , nFightRateState_                  ( (E_ForceRatioStatus)0 )
    , nRulesOfEngagementState_          ( (E_Roe)0 )
    , nRulesOfEngagementPopulationState_( (E_PopulationRoe)0 )
    , nCloseCombatState_                ( (E_MeetingEngagementStatus)0 )
    , bDead_                            ( false )
    , bNeutralized_                     ( false )
    , nOldPosture_                      ( (E_UnitPosture)0 )
    , nCurrentPosture_                  ( (E_UnitPosture)0 )
    , nPostureCompletionPourcentage_    ( 0 )
    , nInstallationState_               ( 0 )
    , nIndirectFireAvailability_        ( (E_FireAvailability)0 )
    , bLoadingState_                    ( false )
    , bHumanTransportersReady_          ( false )
    , bStealthModeEnabled_              ( false )
    , bRadioEmitterSilence_             ( false )
    , bRadioReceiverSilence_            ( false )
    , bCommJammed_                      ( false )
    , knowledgeGroupJammed_             ( 0 )
    , bRadarEnabled_                    ( false )
    , bPrisoner_                        ( false )
    , bUnderground_                     ( false )
    , surrenderedTo_                    ( 0 )
    , bRefugeesManaged_                 ( false )
    , aggregated_                       ( false )
    , fLodgingSatisfactionPercent_      ( 0.0f )
    , fSecuritySatisfactionPercent_     ( 0.0f )
    , fHealthSatisfactionPercent_       ( 0.0f )
    , crowdTransported_                 ( -1 )
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
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Operational state" ),               nRawOpState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Speed" ),                           nSpeed_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Heading" ),                         nDirection_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Critical intelligence" ),           criticalIntelligence_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Underground" ),                     bUnderground_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Info/Neutralized" ),                     bNeutralized_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Stances/Current stance" ),               nCurrentPosture_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Stances/Setup state" ),                  nInstallationState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Communications/Jammed" ),                bCommJammed_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Communications/Radio Emitter silence" ), bRadioEmitterSilence_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Communications/Radio Receiver silence" ),bRadioReceiverSilence_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Decisional state/Operational state" ),   nOpState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Decisional state/Rules of engagement" ), nRulesOfEngagementState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Decisional state/Intention" ),           nCloseCombatState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Decisional state/Force ratio" ),         nFightRateState_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Military state/Prisoner" ),              bPrisoner_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Military state/Surrender" ),             surrenderedTo_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Military state/Refugees picked up" ),    bRefugeesManaged_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Satisfaction/Lodging" ),                 fLodgingSatisfactionPercent_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Satisfaction/Security" ),                fSecuritySatisfactionPercent_ );
    dictionary.Register( entity_, tools::translate( "Attributes", "Satisfaction/Health" ),                  fHealthSatisfactionPercent_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Attributes::DoUpdate( const sword::UnitAttributes& message )
{
    std::set< std::string > updated;

    UPDATE_SUBPROPERTY( message, nDirection_, direction, heading, "Info", updated );
    UPDATE_PROPERTY( message, nRawOpState_, raw_operational_state, "Info", updated );
    UPDATE_PROPERTY( message, nSpeed_, speed, "Info", updated );
    UPDATE_PROPERTY( message, criticalIntelligence_, critical_intelligence, "Info", updated );
    UPDATE_PROPERTY( message, bUnderground_, underground, "Info", updated );

    if( message.has_neutralized() )
    {
        bool bNeutralized = message.neutralized();
        if( bNeutralized != bNeutralized_ )
        {
            bNeutralized_ = bNeutralized;
            updated.insert( "Info" );
            UpdateHierarchies();
        }
    }

    UPDATE_PROPERTY( message, nCurrentPosture_, new_posture, "Stances", updated );
    UPDATE_PROPERTY( message, nInstallationState_, installation, "Stances", updated );

    if( message.has_communications() && message.communications().has_jammed() )
    {
        bool bCommJammed = message.communications().jammed();
        if( bCommJammed != bCommJammed_ )
        {
            bCommJammed_ = bCommJammed;
            updated.insert( "Communications" );
        }
    }

    UPDATE_PROPERTY( message, bRadioEmitterSilence_, radio_emitter_disabled, "Communications", updated );
    UPDATE_PROPERTY( message, bRadioReceiverSilence_, radio_receiver_disabled, "Communications", updated );

    if( message.has_operational_state() )
    {
        E_OperationalStatus nOpState = (E_OperationalStatus)message.operational_state();
        if( nOpState != nOpState_ )
        {
            nOpState_ = nOpState;
            updated.insert( "Decisional state" );
            UpdateHierarchies();
        }
    }

    UPDATE_PROPERTY( message, nRulesOfEngagementState_, roe, "Decisional state", updated );
    UPDATE_PROPERTY( message, nCloseCombatState_, meeting_engagement, "Decisional state", updated );
    UPDATE_PROPERTY( message, nFightRateState_, force_ratio, "Decisional state", updated );
    UPDATE_PROPERTY( message, bPrisoner_, prisoner, "Military state", updated );

    if( message.has_surrendered_unit() )
    {
        const kernel::Team_ABC* surrenderedTo = teamResolver_.Find( message.surrendered_unit().id() );
        if( surrenderedTo != surrenderedTo_ )
        {
            surrenderedTo_ = surrenderedTo;
            updated.insert( "Military state" );
        }
    }

    UPDATE_PROPERTY( message, bRefugeesManaged_, refugees_managed, "Military state", updated );

    if( message.has_satisfaction() )
    {
        if( message.satisfaction().has_lodging() )
        {
            float fLodgingSatisfactionPercent = 100.0f * message.satisfaction().lodging();
            if( fLodgingSatisfactionPercent_ != fLodgingSatisfactionPercent )
            {
                fLodgingSatisfactionPercent_ = fLodgingSatisfactionPercent;
                updated.insert( "Satisfaction" );
            }
        }
        if( message.satisfaction().has_safety() )
        {
            float fSecuritySatisfactionPercent = 100.0f * message.satisfaction().safety();
            if( fSecuritySatisfactionPercent_ != fSecuritySatisfactionPercent )
            {
                fSecuritySatisfactionPercent_ = fSecuritySatisfactionPercent;
                updated.insert( "Satisfaction" );
            }
        }
        if( message.satisfaction().has_access_to_health_care() )
        {
            float fHealthSatisfactionPercent = 100.0f * message.satisfaction().access_to_health_care();
            if( fHealthSatisfactionPercent_ != fHealthSatisfactionPercent )
            {
                fHealthSatisfactionPercent_ = fHealthSatisfactionPercent;
                updated.insert( "Satisfaction" );
            }
        }
    }

    if( message.has_communications() && message.communications().has_knowledge_group() )
        knowledgeGroupJammed_ = message.communications().knowledge_group().id();

    if( message.has_position() )
        vPos_ = converter_.ConvertToXY( message.position() );

    if( message.has_indirect_fire_availability() )
        nIndirectFireAvailability_ = (E_FireAvailability)message.indirect_fire_availability();

    if( message.has_old_posture()  )
        nOldPosture_ = (E_UnitPosture)message.old_posture();

    if( message.has_posture_transition() )
        nPostureCompletionPourcentage_ = message.posture_transition();

    if( message.has_dead() )
        bDead_ = message.dead();

    if( message.has_roe_crowd() )
        nRulesOfEngagementPopulationState_ = (E_PopulationRoe)message.roe_crowd();

    if( message.has_embarked() )
        bLoadingState_ = message.embarked();

    if( message.has_transporters_available()  )
        bHumanTransportersReady_ = message.transporters_available();

    if( message.has_stealth() )
        bStealthModeEnabled_ = message.stealth();

    if( message.has_height() )
        nAltitude_ = message.height();

    if( message.has_radar_active() )
        bRadarEnabled_ = message.radar_active();

    if( message.has_transported_crowd() )
        crowdTransported_ = message.transported_crowd();

    BOOST_FOREACH( const std::string& content, updated )
        controller_.Update( kernel::DictionaryUpdated( entity_, tools::translate( "Attributes", content.c_str() ) ) );

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
                .Display( tools::translate( "Attributes", "Operational state:" ),     nRawOpState_ * Units::percentage )
                .Display( tools::translate( "Attributes", "Dead:" ),                  bDead_ )
                .Display( tools::translate( "Attributes", "Neutralized:" ),           bNeutralized_ )
                .Display( tools::translate( "Attributes", "Speed:" ),                 nSpeed_ * Units::kilometersPerHour )
                .Display( tools::translate( "Attributes", "Heading:" ),               nDirection_ * Units::degrees )
                .Display( tools::translate( "Attributes", "Underground:" ),           bUnderground_ )
                .Display( tools::translate( "Attributes", "Critical intelligence:" ), criticalIntelligence_ )
                .Display( tools::translate( "Attributes", "Height:" ),                nAltitude_  * Units::meters )
                .Display( tools::translate( "Attributes", "Troops:" ),                bLoadingState_ ? tools::translate( "Attributes", "on-board" ) : tools::translate( "Attributes", "off-board" ) )
                .Display( tools::translate( "Attributes", "Human transportation:" ),  bHumanTransportersReady_ ? tools::translate( "Attributes", "Available" ) : tools::translate( "Attributes", "Unavailable" ) );

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
                .Display( tools::translate( "Attributes", "Radio Emitter silence:" ), bRadioEmitterSilence_ )
                .Display( tools::translate( "Attributes", "Radio Receiver silence:" ), bRadioReceiverSilence_ );

    displayer.Group( tools::translate( "Attributes", "Decisional state" ) )
                .Display( tools::translate( "Attributes", "Operational state:" ), nOpState_ )
                .Display( tools::translate( "Attributes", "Rules of engagement:" ), nRulesOfEngagementState_ )
                .Display( tools::translate( "Attributes", "Crowd RoE:" ), nRulesOfEngagementPopulationState_ )
                .Display( tools::translate( "Attributes", "Force ratio:" ), nFightRateState_ )
                .Display( tools::translate( "Attributes", "Indirect fire availability:" ), nIndirectFireAvailability_ )
                .Display( tools::translate( "Attributes", "Intention:" ), nCloseCombatState_ );

    displayer.Group( tools::translate( "Attributes", "Military state" ) )
            .Display( tools::translate( "Attributes", "Prisoner:" ), bPrisoner_ )
            .Display( tools::translate( "Attributes", "Surrender:" ), surrenderedTo_ )
            .Display( tools::translate( "Attributes", "Refugees picked up:" ), bRefugeesManaged_ );

    displayer.Group( tools::translate( "Attributes", "Satisfaction" ) )
        .Display( tools::translate( "Attributes", "Lodging:" ), fLodgingSatisfactionPercent_ )
        .Display( tools::translate( "Attributes", "Security:" ), fSecuritySatisfactionPercent_ )
        .Display( tools::translate( "Attributes", "Health:" ), fHealthSatisfactionPercent_ );
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
void Attributes::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Attributes", "Speed:" ) , nSpeed_ * Units::kilometersPerHour )
             .Display( tools::translate( "Attributes", "Height:" ), nAltitude_ * Units::meters )
             .Display( tools::translate( "Attributes", "Troops:" ), bLoadingState_ ? tools::translate( "Attributes", "on-board" ) : tools::translate( "Attributes", "off-board" ) );
    if( crowdTransported_!= -1 )
        displayer.Display( tools::translate( "Attributes", "Crowd transported:" ), crowdTransported_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Attributes::Draw( const Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( aggregated_
    || ! ( bDead_ || bRadioReceiverSilence_ || bRadioEmitterSilence_ || bRadarEnabled_ || bCommJammed_ || bUnderground_ )
    || ! viewport.IsHotpointVisible() )
        return;

    glPushAttrib( GL_CURRENT_BIT );
    glColor3f( 1, 1, 1 );
    if( bDead_ )
        tools.DrawIcon( xpm_skull, where, 150.f );
    if( bRadioEmitterSilence_ )
        tools.DrawIcon( xpm_silence_radio_outgoing, where, 150.f );
    if( bRadarEnabled_ )
        tools.DrawIcon( xpm_radars_on, where, 150.f );
    if( bCommJammed_ )
        tools.DrawIcon( xpm_brouillage, where, 150.f );
    if( bUnderground_ )
        tools.DrawIcon( xpm_underground, where, 150.f );
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

// -----------------------------------------------------------------------------
// Name: Attributes::UpdateHierarchies
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void Attributes::UpdateHierarchies()
{
    if( const kernel::TacticalHierarchies* pTactical = entity_.Retrieve< kernel::TacticalHierarchies >() )
    {
        controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = entity_.Retrieve< kernel::CommunicationHierarchies >() )
            controller_.Update( *pCommunication );
        else if( const kernel::CommunicationHierarchies* pCommunication = pTactical->GetTop().Retrieve< kernel::CommunicationHierarchies >() )
            controller_.Update( *pCommunication );
    }
}
