// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledge.h"
#include "Diplomacies.h"
#include "PointingKnowledges.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
#include "protocol/Protocol.h"

using namespace kernel;

E_PerceptionResult AgentKnowledge::levelPerception_ = eIdentification;
E_PerceptionResult AgentKnowledge::partialNaturePerception_ = eRecognition;
E_PerceptionResult AgentKnowledge::fullNaturePerception_ = eIdentification;

namespace
{
    E_PerceptionResult ConvertToPerception( const std::string& level )
    {
        if( level == "detection" )
            return eDetection;
        if( level == "recognition" )
            return eRecognition;
        if( level == "identification" )
            return eIdentification;
        if( level == "never" )
            return eNotSeen;
        return static_cast< E_PerceptionResult >( -1 );
    }

    void ReadAvailability( xml::xistream& xis )
    {
        const std::string type = xis.attribute< std::string >( "type" );
        const E_PerceptionResult perception = ConvertToPerception( xis.attribute< std::string >( "level" ) );
        if( perception != static_cast< E_PerceptionResult >( -1 ) )
        {
            if( type == "level" )
                AgentKnowledge::levelPerception_ = perception;
            else if( type == "nature-partial" )
                AgentKnowledge::partialNaturePerception_ = perception;
            else if( type == "nature-full" )
                AgentKnowledge::fullNaturePerception_ = perception;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::LoadPerceptionAvaibilities
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
void AgentKnowledge::LoadPerceptionAvaibilities( const tools::ExerciseConfig& config )
{
    config.GetPhyLoader().LoadPhysicalFile( "decisional",
        [&]( xml::xistream& xis )
        {
            xis >> xml::start( "decisional" )
                >> xml::optional >> xml::start( "perception" )
                   >> xml::list( "availability", &ReadAvailability );
        });
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::PurgePerceptionAvaibilities
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
void AgentKnowledge::PurgePerceptionAvaibilities()
{
    levelPerception_ = eIdentification;
    partialNaturePerception_ = eRecognition;
    fullNaturePerception_ = eIdentification;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const KnowledgeGroup_ABC& group,
                                const sword::UnitKnowledgeCreation& message,
                                Controller& controller,
                                const CoordinateConverter_ABC& converter,
                                const tools::Resolver_ABC< Agent_ABC >& resolver,
                                const tools::Resolver_ABC< Team_ABC >& teamResolver,
                                const kernel::Profile_ABC& profile )
    : EntityImplementation< AgentKnowledge_ABC >( controller, message.knowledge().id(), "", []( const AgentKnowledge_ABC& ){ return false; } )
    , converter_   ( converter )
    , resolver_    ( resolver )
    , teamResolver_( teamResolver )
    , group_       ( group )
    , profile_     ( profile )
    , realAgent_   ( resolver_.Get( message.unit().id() ) )
    , team_        ( 0 )
    , nDirection_  ( 0u, false )
    , nSpeed_      ( 0u, false )
    , nEtatOps_    ( 0u, false )
    , nCurrentPerceptionLevel_( eNotSeen, false )
    , nMaxPerceptionLevel_( eNotSeen, false )
    , nLevel_      ( eNatureLevel_None )
    , bIsPC_       ( false, false )
    , surrenderedTo_( 0, false )
    , bPrisonner_  ( false, false )
    , bRefugies_   ( false, false )
    , nRelevance_  ( 0u, false )
    , criticalIntelligence_( "", false )
    , posture_     ( eUnitPosture_PostureMouvement )
{
    fullSymbol_  = realAgent_.GetType().GetSymbol();
    moveSymbol_ = realAgent_.GetType().GetMoveSymbol();
    staticSymbol_ = realAgent_.GetType().GetStaticSymbol();
    UpdateSymbol();
    AddExtension( *this );
    realAgent_.Get< PointingKnowledges >().Add( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    PointingKnowledges* pointing = realAgent_.Retrieve< PointingKnowledges >();
    if( pointing )
        pointing->Remove( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::DoUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentKnowledge::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    if( message.has_max_identification_level() )
        nMaxPerceptionLevel_ = (E_PerceptionResult)( 3 - message.max_identification_level() );

    if( message.has_identification_level() )
    {
        // $$$$ AGE 2005-03-23: !! Les enums message et sim ne correspondent pas...
        nCurrentPerceptionLevel_ = (E_PerceptionResult)( 3 - message.identification_level() );
        if( nCurrentPerceptionLevel_ > nMaxPerceptionLevel_ )
            nMaxPerceptionLevel_ = nCurrentPerceptionLevel_;
    }

    if( message.has_operational_state() )
        nEtatOps_ = message.operational_state();

    if( message.has_direction() )
        nDirection_ = message.direction().heading();

    if( message.has_speed() )
        nSpeed_ = message.speed();

    if( message.has_party() )
        team_ = & teamResolver_.Get( message.party().id() );

    if( message.has_command_post() )
        bIsPC_ = message.command_post() != 0;

    if( message.has_pertinence() )
        nRelevance_ = message.pertinence();

    if( message.has_prisoner() )
        bPrisonner_ = message.prisoner() != 0;

    if( message.has_surrendered_unit() )
        surrenderedTo_ = teamResolver_.Find( message.surrendered_unit().id() );

    if( message.has_refugees_managed() )
        bRefugies_ = message.refugees_managed() != 0;

    if( message.has_critical_intelligence() )
        criticalIntelligence_ = message.critical_intelligence();

    if( message.has_posture() )
        posture_ = static_cast< E_UnitPosture >( message.posture() );

    UpdateSymbol();

    Touch();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetEntity
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
const kernel::Agent_ABC* AgentKnowledge::GetEntity() const
{
    return &realAgent_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long AgentKnowledge::GetEntityId() const
{
    return realAgent_.GetId();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetTeam
// Created: NLD 2010-11-03
// -----------------------------------------------------------------------------
const kernel::Team_ABC* AgentKnowledge::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetOwner
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const kernel::KnowledgeGroup_ABC& AgentKnowledge::GetOwner() const
{
    return group_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetName
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
QString AgentKnowledge::GetName() const
{
    if( profile_.IsVisible( realAgent_ ) )
        return realAgent_.GetName();
    return currentNature_.empty()
        ? tools::translate( "AgentKnowledge", "unknown %1" ).arg( id_ )
        : tools::translate( "AgentKnowledge", "%1 %2" ).arg( currentNature_.c_str() ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetSymbol
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
std::string AgentKnowledge::GetSymbol() const
{
    return currentSymbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void AgentKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentKnowledge", "Identifier:" ), id_ );
    if( profile_.IsVisible( realAgent_ ) )
        displayer.Display( tools::translate( "AgentKnowledge", "Associated agent:" ), realAgent_ );
    else
        displayer.Display( tools::translate( "AgentKnowledge", "Associated agent:" ), GetName() );
    displayer.Display( tools::translate( "AgentKnowledge", "Location:" ), strPosition_ )
             .Display( tools::translate( "AgentKnowledge", "Heading:" ), nDirection_ * Units::degrees )
             .Display( tools::translate( "AgentKnowledge", "Speed:" ), nSpeed_ * Units::kilometersPerHour )
             .Display( tools::translate( "AgentKnowledge", "Operational state:" ), nEtatOps_ * Units::percentage )
             .Display( tools::translate( "AgentKnowledge", "Perception level:" ), nCurrentPerceptionLevel_  )
             .Display( tools::translate( "AgentKnowledge", "Max perception level:" ), nMaxPerceptionLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Side:" ), team_  )
             .Display( tools::translate( "AgentKnowledge", "Level:" ), nLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Nature:" ), currentNature_ )
             .Display( tools::translate( "AgentKnowledge", "Surrender:" ), surrenderedTo_ )
             .Display( tools::translate( "AgentKnowledge", "Prisoner:" ), bPrisonner_ )
             .Display( tools::translate( "AgentKnowledge", "Refugees picked up:" ), bRefugies_ )
             .Display( tools::translate( "AgentKnowledge", "Command post:" ), bIsPC_ )
             .Display( tools::translate( "AgentKnowledge", "Relevance:" ), nRelevance_ * Units::percentage )
             .Display( tools::translate( "AgentKnowledge", "Critical intelligence:" ), criticalIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::DisplayInTooltip
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void AgentKnowledge::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentKnowledge", "Perception level:" ), nCurrentPerceptionLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Side:" ), team_  )
             .Display( tools::translate( "AgentKnowledge", "Nature:" ), currentNature_ )
             .Display( tools::translate( "AgentKnowledge", "Level:" ), nLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Operational state:" ), nEtatOps_ * Units::percentage )
             .Display( tools::translate( "AgentKnowledge", "Relevance:" ), nRelevance_ * Units::percentage )
             .Display( tools::translate( "AgentKnowledge", "Critical intelligence:" ), criticalIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void AgentKnowledge::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Draw
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void AgentKnowledge::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        const boost::tuple< bool, bool, bool > backupState = tools.UnSelect();
        unsigned int direction = nDirection_.IsSet() ? (uint) nDirection_ : 0;
        bool isMoving = ( posture_ <= eUnitPosture_PostureArret );
        float width = isMoving? 0.f : realAgent_.GetType().GetWidth();
        float depth = isMoving? realAgent_.GetType().GetLength() : realAgent_.GetType().GetDepth();
        tools.DrawUnitSymbol( currentSymbol_, moveSymbol_, staticSymbol_, realAgent_.GetType().GetLevelSymbol(), isMoving, where, -1, direction, width, depth );
        bool app6 = isMoving ? moveSymbol_.empty() : staticSymbol_.empty();
        if( app6 && nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection )
        {
            tools.DrawApp6SymbolFixedSize( realAgent_.GetType().GetLevelSymbol(), where, -1, 0 );
            if( bIsPC_.IsSet() && bIsPC_ )
                tools.DrawApp6SymbolFixedSize( realAgent_.GetType().GetHQSymbol(), where, -1, 0 );
        }
        tools.Select( backupState.get< 0 >(), backupState.get< 1 >(), backupState.get< 2 >() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void AgentKnowledge::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

namespace
{
    const kernel::Karma& GetKarma( const kernel::Team_ABC* team )
    {
        if( team )
            if( const Diplomacies_ABC* diplomacy = team->Retrieve< Diplomacies_ABC >() )
                return diplomacy->GetKarma();
        return Karma::unknown_;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::UpdateSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void AgentKnowledge::UpdateSymbol()
{
    const E_PerceptionResult perception = nMaxPerceptionLevel_.IsSet() ? nMaxPerceptionLevel_ : eDetection;
    App6Symbol::E_LevelFilter filter = App6Symbol::eLevelFilter_Nothing;
    if( fullNaturePerception_ != eNotSeen && perception >= fullNaturePerception_ )
        filter = App6Symbol::eLevelFilter_Full;
    else if( partialNaturePerception_ != eNotSeen && perception >= partialNaturePerception_ )
        filter = App6Symbol::eLevelFilter_Partial;
    currentSymbol_ = fullSymbol_;
    const auto& teamKarma = GetKarma( team_ );
    App6Symbol::SetKarma( currentSymbol_, teamKarma );
    App6Symbol::SetKarma( moveSymbol_, teamKarma );
    App6Symbol::SetKarma( staticSymbol_, teamKarma );
    App6Symbol::FilterPerceptionLevel( currentSymbol_, filter );
    currentNature_ = App6Symbol::FilterNature( realAgent_.GetType().GetNature().GetNature(), filter );
    if( levelPerception_ != eNotSeen && nLevel_ == eNatureLevel_None && nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ >= levelPerception_ )
        nLevel_ = tools::NatureLevelFromString( realAgent_.GetType().GetNature().GetLevel() );
}
