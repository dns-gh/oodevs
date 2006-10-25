// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
{
    fullSymbol_  = realAgent_.GetType().GetSymbol();
    levelSymbol_ = realAgent_.GetType().GetLevelSymbol();
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
        bSurrendered_ = message.rendu;

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
// Name: AgentKnowledge::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString AgentKnowledge::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void AgentKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentKnowledge", "Id:" ), id_ )
             .Display( tools::translate( "AgentKnowledge", "Agent associ�:" ), realAgent_ )
             .Display( tools::translate( "AgentKnowledge", "Position:" ), strPosition_ )
             .Display( tools::translate( "AgentKnowledge", "Direction:" ), nDirection_ * Units::degrees )
             .Display( tools::translate( "AgentKnowledge", "Vitesse:" ), nSpeed_ * Units::kilometersPerHour )
             .Display( tools::translate( "AgentKnowledge", "Etat ops.:" ), nEtatOps_ * Units::percentage )
             .Display( tools::translate( "AgentKnowledge", "Niveau de perception:" ), nCurrentPerceptionLevel_  )
             .Display( tools::translate( "AgentKnowledge", "Niveau max de perception:" ), nMaxPerceptionLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Camp:" ), team_  )
             .Display( tools::translate( "AgentKnowledge", "Niveau:" ), nLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Nature:" ), currentNature_ )
             .Display( tools::translate( "AgentKnowledge", "Rendu:" ), bSurrendered_ )
             .Display( tools::translate( "AgentKnowledge", "Fait prisonnier:" ), bPrisonner_ )
             .Display( tools::translate( "AgentKnowledge", "R�fugi�s pris en compte:" ), bRefugies_ )
             .Display( tools::translate( "AgentKnowledge", "PC:" ), bIsPC_ )
             .Display( tools::translate( "AgentKnowledge", "Pertinence:" ), nRelevance_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Draw
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void AgentKnowledge::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
    {
        const bool backupState = tools.Select( false );
        tools.DrawApp6Symbol( currentSymbol_, where );
        if( nMaxPerceptionLevel_.IsSet() && nMaxPerceptionLevel_ > eDetection )
            tools.DrawApp6Symbol( levelSymbol_, where );
        tools.Select( backupState );
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
    case eDetection:      return 0; // nothing
    case eRecognition:    return 2; // side + category + weapon
    case eIdentification: return 9; // all
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::TeamCharacter
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
char AgentKnowledge::TeamCharacter( E_PerceptionResult perception ) const
{
    ASN1T_EnumDiplomatie diplo = EnumDiplomatie::inconnu;
    if( team_ && perception > eDetection )
        if( const Diplomacies* diplomacy = team_->Retrieve< Diplomacies >() )
            diplo = diplomacy->GetRelationship( group_ );
    switch( diplo )
    {
    default:
    case EnumDiplomatie::inconnu: return 'u';
    case EnumDiplomatie::ami:     return 'f';
    case EnumDiplomatie::ennemi:  return 'h';
    case EnumDiplomatie::neutre:  return 'n';
    }
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
    const unsigned int toKeep = ElementsToKeep( perception); 
    const char teamChar = TeamCharacter( perception );
    
    currentSymbol_ = fullSymbol_;
    std::string::size_type pos = currentSymbol_.find_first_of( '*' );
    currentSymbol_[ pos ] = teamChar;
    currentSymbol_ = currentSymbol_.substr( 0, pos + toKeep + 1 );

    currentNature_ = Strip( realAgent_.GetType().GetNature().GetNature(), toKeep ); // $$$$ AGE 2006-10-25: 
}
