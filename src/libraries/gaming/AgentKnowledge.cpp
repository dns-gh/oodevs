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
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

IDManager AgentKnowledge::idManager_( 158 );

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const KnowledgeGroup_ABC& group, const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< Team_ABC >& teamResolver )
    : controller_  ( controller )
    , converter_   ( converter )
    , resolver_    ( resolver )
    , teamResolver_( teamResolver )
    , group_       ( group )
    , nID_         ( message.oid_connaissance )
    , realAgent_   ( resolver_.Get( message.oid_unite_reelle ) )
    , team_        ( 0 )
{
    RegisterSelf( *this );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    controller_.Delete( *this );
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

    if( message.m.nature_niveauPresent )
        nLevel_ = (E_NatureLevel)message.nature_niveau;

    if( message.m.nature_armePresent )
        nWeapon_ = (E_UnitNatureWeapon)message.nature_arme;

    if( message.m.nature_specialisationPresent )
        nSpecialization_ = (E_UnitNatureSpecialization)message.nature_specialisation;

    if( message.m.nature_qualificationPresent )
        nQualifier_ = (E_UnitNatureQualifier)message.nature_qualification;

    if( message.m.nature_categoriePresent )
        nCategory_ = (E_UnitNatureCategory)message.nature_categorie;

    if( message.m.nature_mobilitePresent )
        nMobility_ = (E_UnitNatureMobility)message.nature_mobilite;

    if( message.m.nature_pcPresent )
        bIsPC_ = message.nature_pc;

    if( message.m.pertinencePresent )
        nRelevance_ = message.pertinence;

    if( message.m.capacite_missionPresent )
        nCapacity_ = E_UnitCapaciteMission( message.capacite_mission );

    if( message.m.prisonnierPresent )
        bPrisonner_ = message.prisonnier;

    if( message.m.renduPresent )
        bSurrendered_ = message.rendu;

    if( message.m.refugie_pris_en_comptePresent )
        bRefugies_ = message.refugie_pris_en_compte;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetId
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
unsigned long AgentKnowledge::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetRealAgent
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Agent_ABC& AgentKnowledge::GetRealAgent() const
{
    return realAgent_;
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
// Name: AgentKnowledge::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void AgentKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentKnowledge", "Id:" ), nID_ )
             .Display( tools::translate( "AgentKnowledge", "Agent associ�:" ), realAgent_ )
             .Display( tools::translate( "AgentKnowledge", "Position:" ), strPosition_ )
             .Display( tools::translate( "AgentKnowledge", "Direction:" ), nDirection_ * Units::degrees )
             .Display( tools::translate( "AgentKnowledge", "Vitesse:" ), nSpeed_ * Units::metersPerSecond )
             .Display( tools::translate( "AgentKnowledge", "Etat ops.:" ), nEtatOps_ )
             .Display( tools::translate( "AgentKnowledge", "Niveau de perception:" ), nCurrentPerceptionLevel_  )
             .Display( tools::translate( "AgentKnowledge", "Niveau max de perception:" ), nMaxPerceptionLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Camp:" ), team_  )
             .Display( tools::translate( "AgentKnowledge", "Niveau:" ), nLevel_ )
             .Display( tools::translate( "AgentKnowledge", "Arme:" ), nWeapon_ )
             .Display( tools::translate( "AgentKnowledge", "Sp�cialisation:" ), nSpecialization_ )
             .Display( tools::translate( "AgentKnowledge", "Qualification:" ), nQualifier_  )
             .Display( tools::translate( "AgentKnowledge", "Cat�gorie:" ), nCategory_ )
             .Display( tools::translate( "AgentKnowledge", "Mobilit�:" ), nMobility_  )
             .Display( tools::translate( "AgentKnowledge", "Capacit� mission:" ), nCapacity_ )
             .Display( tools::translate( "AgentKnowledge", "Rendu:" ), bSurrendered_ )
             .Display( tools::translate( "AgentKnowledge", "Fait prisonnier:" ), bPrisonner_ )
             .Display( tools::translate( "AgentKnowledge", "R�fugi�s pris en compte:" ), bRefugies_ )
             .Display( tools::translate( "AgentKnowledge", "PC:" ), bIsPC_ )
             .Display( tools::translate( "AgentKnowledge", "Pertinence:" ), nRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool AgentKnowledge::IsInTeam( const Team_ABC& team ) const
{
    return group_.GetTeam() == team;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::KnowledgeIsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool AgentKnowledge::KnowledgeIsInTeam( const Team_ABC& team ) const
{
    return team_ && *team_ == team;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Draw
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void AgentKnowledge::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( nCurrentPerceptionLevel_.IsSet() && E_PerceptionResult( nCurrentPerceptionLevel_ ) > eDetection )
    {
        const bool backupState = tools.Select( false );
        realAgent_.Entity_ABC::Draw( where, viewport, tools );
        tools.Select( backupState );
    }
    else
        tools.DrawApp6Symbol( "roles/Unknown.svg", where );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetKnowledgeTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
const Team_ABC* AgentKnowledge::GetKnowledgeTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetKnowledgeGroup
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const KnowledgeGroup_ABC& AgentKnowledge::GetKnowledgeGroup() const
{
    return group_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void AgentKnowledge::Select( ActionController& controller ) const
{
     controller.Select( *this );
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledge::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void AgentKnowledge::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void AgentKnowledge::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}
