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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

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
    return nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection
        ? tools::translate( "AgentKnowledge", "unknown %1 %2" ).arg( currentNature_.c_str() ).arg( id_ )
        : tools::translate( "AgentKnowledge", "unknown %1" ).arg( id_ );
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
void AgentKnowledge::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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
void AgentKnowledge::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::TeamCharacter
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
const kernel::Karma& AgentKnowledge::TeamKarma( E_PerceptionResult perception ) const
{
    if( team_ && perception > eDetection )
        if( const Diplomacies_ABC* diplomacy = team_->Retrieve< Diplomacies_ABC >() )
            return diplomacy->GetKarma();
    return Karma::unknown_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::UpdateSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void AgentKnowledge::UpdateSymbol()
{
    const E_PerceptionResult perception = nMaxPerceptionLevel_.IsSet() ? nMaxPerceptionLevel_ : eDetection;
    currentSymbol_ = fullSymbol_;
    App6Symbol::SetKarma( currentSymbol_, TeamKarma( perception ) );
    App6Symbol::SetKarma( moveSymbol_, TeamKarma( perception ) );
    App6Symbol::SetKarma( staticSymbol_, TeamKarma( perception ) );
    App6Symbol::FilterPerceptionLevel( currentSymbol_, perception );
    currentNature_ = App6Symbol::FilterNature( realAgent_.GetType().GetNature().GetNature(), perception );
    if( nLevel_ == eNatureLevel_None && nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection )
        nLevel_ = tools::NatureLevelFromString( realAgent_.GetType().GetNature().GetLevel() );
}
