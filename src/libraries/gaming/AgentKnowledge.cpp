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

#include "ASN_Messages.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "Tools.h"
#include "Diplomacies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const KnowledgeGroup_ABC& group, const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< Team_ABC >& teamResolver )
    : EntityImplementation< AgentKnowledge_ABC >( controller, message.oid_connaissance, "" )
    , converter_   ( converter )
    , resolver_    ( resolver )
    , teamResolver_( teamResolver )
    , group_       ( group )
    , realAgent_   ( resolver_.Get( message.oid_unite_reelle ) )
    , team_        ( 0 )
    , nLevel_      ( eNatureLevel_None )
{
    fullSymbol_  = realAgent_.GetType().GetSymbol();
    UpdateSymbol();
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::DoUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentKnowledge::DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    if( message.m.identification_levelPresent )
        // $$$$ AGE 2005-03-23: !! Les enums asn et sim ne correspondent pas...
        nCurrentPerceptionLevel_ = (E_PerceptionResult)( 3 - message.identification_level );

    if( message.m.max_identification_levelPresent )
        nMaxPerceptionLevel_ = (E_PerceptionResult)( 3 - message.max_identification_level );

    if( message.m.etat_opPresent )
        nEtatOps_ = message.etat_op;

    if( message.m.positionPresent )
        strPosition_ = std::string( (const char*)message.position.data, 15 );

    if( message.m.directionPresent )
        nDirection_ = message.direction;

    if( message.m.speedPresent )
        nSpeed_ = message.speed;

    if( message.m.campPresent )
        team_ = & teamResolver_.Get( message.camp );

    if( message.m.nature_pcPresent )
        bIsPC_ = message.nature_pc;

    if( message.m.pertinencePresent )
        nRelevance_ = message.pertinence;

    if( message.m.prisonnierPresent )
        bPrisonner_ = message.prisonnier;

    if( message.m.renduPresent )
        surrenderedTo_ = teamResolver_.Find( message.rendu );

    if( message.m.refugie_pris_en_comptePresent )
        bRefugies_ = message.refugie_pris_en_compte;

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
// Name: AgentKnowledge::GetRecognizedEntity
// Created: SBO 2006-12-08
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AgentKnowledge::GetRecognizedEntity() const
{
    E_PerceptionResult perception = nMaxPerceptionLevel_.IsSet() ? nMaxPerceptionLevel_ : eDetection;
    return ( team_ && perception > eDetection ) ? &realAgent_ : 0;
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
    return realAgent_.GetName();
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
    displayer.Display( tools::translate( "AgentKnowledge", "Identifier:" ), id_ )
             .Display( tools::translate( "AgentKnowledge", "Associated agent:" ), realAgent_ )
             .Display( tools::translate( "AgentKnowledge", "Location:" ), strPosition_ )
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
             .Display( tools::translate( "AgentKnowledge", "Relevance:" ), nRelevance_ * Units::percentage );
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
             .Display( tools::translate( "AgentKnowledge", "Relevance:" ), nRelevance_ * Units::percentage );
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
void AgentKnowledge::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        const std::pair< bool, bool > backupState = tools.UnSelect();
        tools.DrawApp6Symbol( currentSymbol_, where );
        if( nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection )
        {
            const bool bPc = bIsPC_.IsSet() ? bIsPC_ : false;
            realAgent_.GetType().Draw( where, viewport, tools, bPc );
        }
        tools.Select( backupState.first, backupState.second );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::ElementsToKeep
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
unsigned int AgentKnowledge::ElementsToKeep( E_PerceptionResult perception ) const
{
    switch( perception )
    {
    default:
    case eNotSeen:        
    case eDetection:      return 3; // nothing                  sugpu
    case eRecognition:    return 5; // side + category + weapon shgpuca
    case eIdentification: return 9; // all                      shgpucaaaw
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::TeamCharacter
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
const kernel::Karma& AgentKnowledge::TeamKarma( E_PerceptionResult perception ) const
{
    if( team_ && perception > eDetection )
        if( const Diplomacies* diplomacy = team_->Retrieve< Diplomacies >() )
            return diplomacy->GetKarma();
    return Karma::unknown_;
}

namespace
{
    std::string Strip( const std::string& nature, unsigned int keep )
    {
        QStringList list = QStringList::split( '/',  nature.c_str() );
        while( list.size() > keep )
            list.pop_back();
        QString result = list.join( "/" );
        return result.isNull() ? "" : result.ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::UpdateSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void AgentKnowledge::UpdateSymbol()
{
    E_PerceptionResult perception = nMaxPerceptionLevel_.IsSet() ? nMaxPerceptionLevel_ : eDetection;
    const unsigned int toKeep = ElementsToKeep( perception ); 

    currentSymbol_ = fullSymbol_;
    App6Symbol::SetKarma( currentSymbol_, TeamKarma( perception ) );
    App6Symbol::FilterPerceptionLevel( currentSymbol_, perception );

    currentNature_ = Strip( realAgent_.GetType().GetNature().GetNature(), toKeep - 3 ); // $$$$ AGE 2006-10-25: 

    if( nLevel_ == eNatureLevel_None && nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection )
        nLevel_ = tools::NatureLevelFromString( realAgent_.GetType().GetNature().GetLevel() ); // $$$$ AGE 2006-11-20: 
}
