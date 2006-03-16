//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: AgentKnowledge.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledge.h"

#include "ASN_Messages.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "Units.h"
#include "CoordinateConverter.h"
#include "Agent.h"

IDManager AgentKnowledge::idManager_( 158 );

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, const CoordinateConverter& converter, const Resolver_ABC< Agent >& resolver, const Resolver_ABC< Team >& teamResolver )
    : controller_( controller )
    , converter_( converter )
    , resolver_ ( resolver )
    , teamResolver_( teamResolver )
    , nID_      ( message.oid_connaissance )
    , realAgent_( resolver_.Get( message.oid_unite_reelle ) )
    , team_( 0 )
{
    InterfaceContainer< Extension_ABC >::Register( *this );
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
    {
        strPosition_ = std::string( (const char*)message.position.data, 15 );
        vPosition_ = converter_.ConvertToXY( message.position );
    }

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
Agent& AgentKnowledge::GetRealAgent() const
{
    return realAgent_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetName
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
std::string AgentKnowledge::GetName() const
{
    return realAgent_.GetName();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void AgentKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Id:", nID_ )
             .Display( "Agent associé:", realAgent_ )
             .Display( "Position:", strPosition_ )
             .Display( "Direction:", nDirection_ * Units::degrees )
             .Display( "Vitesse:", nSpeed_ * Units::metersPerSecond )
             .Display( "Etat ops.:", nEtatOps_ )
             .Display( "Niveau de perception:", nCurrentPerceptionLevel_  )
             .Display( "Niveau max de perception:", nMaxPerceptionLevel_ )
             .Display( "Camp:", team_  )
             .Display( "Niveau:", nLevel_ )
             .Display( "Arme:", nWeapon_ )
             .Display( "Spécialisation:", nSpecialization_ )
             .Display( "Qualification:", nQualifier_  )
             .Display( "Catégorie:", nCategory_ )
             .Display( "Mobilité:", nMobility_  )
             .Display( "Capacité mission:", nCapacity_ )
             .Display( "Rendu:", bSurrendered_ )
             .Display( "Fait prisonnier:", bPrisonner_ )
             .Display( "Réfugiés pris en compte:", bRefugies_ )
             .Display( "PC:", bIsPC_ )
             .Display( "Pertinence:", nRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool AgentKnowledge::IsInTeam( const Team& team ) const
{
    return team_ == &team;
}
