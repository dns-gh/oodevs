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
#include "clients_gui/DictionaryUpdated.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/SensorType.h"
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
Attributes::Attributes( kernel::Entity_ABC& entity, Controller& controller, const kernel::DetectionMap& elevation, 
                        const CoordinateConverter_ABC& converter, gui::PropertiesDictionary& dictionary,
                        const tools::Resolver_ABC< Team_ABC >& teamResolver )
    : entity_                           ( entity )
    , controller_                       ( controller )
    , elevation_                        ( elevation )
    , converter_                        ( converter )
    , teamResolver_                     ( teamResolver )
    , vPos_                             ( 0, 0 )
    , nSpeed_                           ( 0 )
    , nAltitude_                        ( 0 )
    , nDirection_                       ( 0 )
    , nSlope_                           ( 0 )
    , nRawOpState_                      ( 0 )
    , nOpState_                         ( eOperationalStatus_Operationnel )
    , nFightRateState_                  ( (E_ForceRatioStatus)0 )
    , nRulesOfEngagementState_          ( (E_Roe)0 )
    , nRulesOfEngagementPopulationState_( (E_PopulationRoe)0 )
    , nCloseCombatState_                ( (E_MeetingEngagementStatus)0 )
    , nOldPosture_                      ( (E_UnitPosture)0 )
    , nCurrentPosture_                  ( (E_UnitPosture)0 )
    , nPostureCompletionPourcentage_    ( 0 )
    , nInstallationState_               ( 0 )
    , nIndirectFireAvailability_        ( (E_FireAvailability)0 )
    , bDead_                            ( false )
    , bNeutralized_                     ( false )
    , bLoadingState_                    ( false )
    , bHumanTransportersReady_          ( false )
    , bStealthModeEnabled_              ( false )
    , bRadioEmitterSilence_             ( false )
    , bRadioReceiverSilence_            ( false )
    , bCommJammed_                      ( false )
    , bRadarEnabled_                    ( false )
    , bPrisoner_                        ( false )
    , bUnderground_                     ( false )
    , bRefugeesManaged_                 ( false )
    , isPC_                             ( false )
    , bAmbianceSafety_                  ( false )
    , surrenderedTo_                    ( 0 )
    , fLodgingSatisfactionPercent_      ( 0.0f )
    , fSecuritySatisfactionPercent_     ( 0.0f )
    , fHealthSatisfactionPercent_       ( 0.0f )
    , cellDistance_                     ( 1.414f * elevation_.GetCellSize() )
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
void Attributes::CreateDictionary( gui::PropertiesDictionary& dictionary ) const
{
    // $$$$ AGE 2006-06-22: unité !
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Operational state" ),               nRawOpState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Dead" ),                            bDead_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Neutralized" ),                     bNeutralized_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Speed" ),                           nSpeed_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Heading" ),                         nDirection_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Underground" ),                     bUnderground_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Neutralized" ),                     bNeutralized_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Troops on board" ),                 bLoadingState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Info/Human transportation available" ),  bHumanTransportersReady_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Stances/Current stance" ),               nCurrentPosture_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Stances/Setup state" ),                  nInstallationState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Stances/Safety attitude" ),              bAmbianceSafety_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Communications/Jammed" ),                bCommJammed_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Communications/Radio Emitter silence" ), bRadioEmitterSilence_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Communications/Radio Receiver silence" ),bRadioReceiverSilence_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Operational state" ),   nOpState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Rules of engagement" ), nRulesOfEngagementState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Crowd RoE" ),           nRulesOfEngagementPopulationState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Intention" ),           nCloseCombatState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Force ratio" ),         nFightRateState_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Decisional state/Indirect fire availability" ), nIndirectFireAvailability_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Military state/Prisoner" ),              bPrisoner_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Military state/Surrender" ),             surrenderedTo_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Military state/Refugees picked up" ),    bRefugeesManaged_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Satisfaction/Lodging" ),                 fLodgingSatisfactionPercent_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Satisfaction/Security" ),                fSecuritySatisfactionPercent_ );
    dictionary.RegisterExtension( entity_, this, tools::translate( "Attributes", "Satisfaction/Health" ),                  fHealthSatisfactionPercent_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Attributes::DoUpdate( const sword::UnitAttributes& message )
{
    std::set< std::string > updated;
    bool mustUpdateHierarchies = false;

    UPDATE_SUBPROPERTY( message, nDirection_, direction, heading, "Info/Heading", updated );
    UPDATE_PROPERTY( message, nRawOpState_, raw_operational_state, "Info/Operational state", updated );
    UPDATE_PROPERTY( message, nSpeed_, speed, "Info/Speed", updated );
    UPDATE_PROPERTY( message, bUnderground_, underground, "Info/Underground", updated );

    if( message.has_neutralized() )
    {
        bool bNeutralized = message.neutralized();
        if( bNeutralized != bNeutralized_ )
        {
            bNeutralized_ = bNeutralized;
            updated.insert( "Info/Neutralized" );
            mustUpdateHierarchies = true;
        }
    }

    UPDATE_PROPERTY( message, nCurrentPosture_, new_posture, "Stances/Current stance", updated );
    UPDATE_PROPERTY( message, nInstallationState_, installation, "Stances/Setup state", updated );
    UPDATE_PROPERTY( message, bAmbianceSafety_, ambiance_safety, "Stances/Safety attitude", updated );

    if( message.has_communications() && message.communications().has_jammed() )
    {
        bool bCommJammed = message.communications().jammed();
        if( bCommJammed != bCommJammed_ )
        {
            bCommJammed_ = bCommJammed;
            updated.insert( "Communications/Jammed" );
        }
    }

    UPDATE_PROPERTY( message, bRadioEmitterSilence_, radio_emitter_disabled, "Communications/Radio Emitter silence", updated );
    UPDATE_PROPERTY( message, bRadioReceiverSilence_, radio_receiver_disabled, "Communications/Radio Receiver silence", updated );

    if( message.has_operational_state() )
    {
        E_OperationalStatus nOpState = (E_OperationalStatus)message.operational_state();
        if( nOpState != nOpState_ )
        {
            nOpState_ = nOpState;
            updated.insert( "Decisional state/Operational state" );
            mustUpdateHierarchies = true;
        }
    }

    UPDATE_PROPERTY( message, nRulesOfEngagementState_, roe, "Decisional state/Rules of engagement", updated );
    UPDATE_PROPERTY( message, nCloseCombatState_, meeting_engagement, "Decisional state/Intention", updated );
    UPDATE_PROPERTY( message, nFightRateState_, force_ratio, "Decisional state/Force ratio", updated );
    UPDATE_PROPERTY( message, bPrisoner_, prisoner, "Military state/Prisoner", updated );

    if( message.has_surrendered_unit() )
    {
        int side = message.surrendered_unit().id();
        const kernel::Team_ABC* surrenderedTo = side == 0 ? 0 : teamResolver_.Find( side );
        if( surrenderedTo != surrenderedTo_ )
        {
            surrenderedTo_ = surrenderedTo;
            updated.insert( "Military state/Surrender" );
        }
    }

    UPDATE_PROPERTY( message, bRefugeesManaged_, refugees_managed, "Military state/Refugees picked up", updated );

    if( message.has_satisfaction() )
    {
        if( message.satisfaction().has_lodging() )
        {
            float fLodgingSatisfactionPercent = 100.0f * message.satisfaction().lodging();
            if( fLodgingSatisfactionPercent_ != fLodgingSatisfactionPercent )
            {
                fLodgingSatisfactionPercent_ = fLodgingSatisfactionPercent;
                updated.insert( "Satisfaction/Lodging" );
            }
        }
        if( message.satisfaction().has_safety() )
        {
            float fSecuritySatisfactionPercent = 100.0f * message.satisfaction().safety();
            if( fSecuritySatisfactionPercent_ != fSecuritySatisfactionPercent )
            {
                fSecuritySatisfactionPercent_ = fSecuritySatisfactionPercent;
                updated.insert( "Satisfaction/Security" );
            }
        }
        if( message.satisfaction().has_access_to_health_care() )
        {
            float fHealthSatisfactionPercent = 100.0f * message.satisfaction().access_to_health_care();
            if( fHealthSatisfactionPercent_ != fHealthSatisfactionPercent )
            {
                fHealthSatisfactionPercent_ = fHealthSatisfactionPercent;
                updated.insert( "Satisfaction/Health" );
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
    {
        bool dead = bDead_;
        bDead_ = message.dead();
        if( dead != bDead_ )
            mustUpdateHierarchies = true;
    }

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
        controller_.Update( gui::DictionaryUpdated( entity_, tools::translate( "Attributes", content.c_str() ) ) );

    if( message.has_headquarters() )
    {
        if( isPC_ != message.headquarters() )
        {
            isPC_ = !isPC_;
            mustUpdateHierarchies = true;
        }
    }

    if( message.has_position() || message.has_direction() )
    {
        const float angle = nDirection_ * 3.14f / 180.f;
        const geometry::Vector2f direction = geometry::Vector2f( std::sin( angle ), std::cos( angle ) );
        const geometry::Point2f vDst = vPos_ + cellDistance_ * direction;
        const float distance = vDst.Distance( vPos_ );
        const double diff = elevation_.ElevationAt( vDst ) - elevation_.ElevationAt( vPos_ );
        nSlope_ = distance == 0 ? 0 : static_cast< int >( 100 * diff / distance );
    }

    if( mustUpdateHierarchies )
        UpdateHierarchies();
    controller_.Update( *this );
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
             .Display( tools::translate( "Attributes", "Slope:" ) , nSlope_ * Units::percentage )
             .Display( tools::translate( "Attributes", "Height:" ), nAltitude_ * Units::meters )
             .Display( tools::translate( "Attributes", "Troops:" ), bLoadingState_ ? tools::translate( "Attributes", "on-board" ) : tools::translate( "Attributes", "off-board" ) );
    if( crowdTransported_!= -1 )
        displayer.Display( tools::translate( "Attributes", "Crowd transported:" ), crowdTransported_ );
}

// -----------------------------------------------------------------------------
// Name: Attributes::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Attributes::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    const bool bEmptyGasTank = entity_.Get< kernel::Dotations_ABC >().IsEmptyGasTank();
    if( entity_.IsAnAggregatedSubordinate()
    || ! ( bDead_ || bRadioReceiverSilence_ || bRadioEmitterSilence_ || bRadarEnabled_ || bCommJammed_ || bUnderground_ || bEmptyGasTank )
    || ! viewport.IsHotpointVisible() || !tools.GetOptions().ShouldDisplay( "UnitDetails" ) )
        return;
    glPushAttrib( GL_CURRENT_BIT );
    glColor4f( 1, 1, 1, tools.GetCurrentAlpha() );
    if( bDead_ )
        tools.DrawIcon( xpm_skull, where, 150.f, gui::GLView_ABC::pixels );
    if( bRadioEmitterSilence_ )
        tools.DrawIcon( xpm_silence_radio_outgoing, where, 150.f, gui::GLView_ABC::pixels );
    if( bRadarEnabled_ )
        tools.DrawIcon( xpm_radars_on, where, 150.f, gui::GLView_ABC::pixels );
    if( bCommJammed_ )
        tools.DrawIcon( xpm_brouillage, where, 150.f, gui::GLView_ABC::pixels );
    if( bUnderground_ )
        tools.DrawIcon( xpm_underground, where, 150.f, gui::GLView_ABC::pixels );
    if( bEmptyGasTank )
        tools.DrawIcon( xpm_gas, where, 150.f, gui::GLView_ABC::pixels );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Attributes::GetDistanceModificator
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
float Attributes::GetDistanceModificator( const kernel::SensorType& sensorType ) const
{
    const std::vector< float >& factors = sensorType.GetPostureSourceFactors();
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
