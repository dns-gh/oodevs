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

#include "App.h"
#include "World.h"
#include "ASN_Messages.h"
#include "AgentManager.h"
#include "Model.h"
#include "AgentsModel.h"
#include "Controller.h"

IDManager AgentKnowledge::idManager_( 158 );

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, const Resolver_ABC< Agent >& resolver  )
    : controller_( controller )
    , resolver_ ( resolver )
    , nID_      ( message.oid_connaissance )
    , realAgent_( resolver_.Get( message.oid_unite_reelle ) )
{
    InterfaceContainer< Extension_ABC >::Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    // NOTHING
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
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.position.data, vPosition_ );
    }

    if( message.m.directionPresent )
        nDirection_ = message.direction;

    if( message.m.speedPresent )
        nSpeed_ = message.speed;

    if( message.m.campPresent )
        nTeam_ = message.camp;

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

    if( message.m.perception_par_compagniePresent )
    {
        automatePerceptionMap_.Set();
        automatePerceptionMap_.Data().clear();
        for( uint i = 0; i < message.perception_par_compagnie.n; ++i )
        {
            Agent* pAgent = & resolver_.Get( message.perception_par_compagnie.elem[i].oid_compagnie );
            automatePerceptionMap_.Data().insert( std::make_pair( pAgent, (E_PerceptionResult)( 3 - message.perception_par_compagnie.elem[i].identification_level ) ) ); //$$$ DEGUEULASSE
        }
    }

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
