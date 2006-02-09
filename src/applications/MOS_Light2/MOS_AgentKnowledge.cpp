//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: MOS_AgentKnowledge.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentKnowledge.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ASN_Messages.h"
#include "MOS_AgentManager.h"

MOS_IDManager MOS_AgentKnowledge::idManager_( 158 );

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentKnowledge::MOS_AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg, MOS_Gtia& owner )
    : nID_                    ( asnMsg.oid_connaissance )
    , owner_                  ( owner )
    , realAgent_              ( *MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_unite_reelle ) )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentKnowledge::~MOS_AgentKnowledge()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentKnowledge::Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.identification_levelPresent )
        // $$$$ AGE 2005-03-23: !! Les enums asn et sim ne correspondent pas...
        nCurrentPerceptionLevel_ = (E_PerceptionResult)( 3 - asnMsg.identification_level );

    if( asnMsg.m.max_identification_levelPresent )
        nMaxPerceptionLevel_ = (E_PerceptionResult)( 3 - asnMsg.max_identification_level );

    if( asnMsg.m.etat_opPresent )
        nEtatOps_ = asnMsg.etat_op;

    if( asnMsg.m.positionPresent )
    {
        strPosition_ = std::string( (const char*)asnMsg.position.data, 15 );
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, vPosition_ );
    }

    if( asnMsg.m.directionPresent )
        nDirection_ = asnMsg.direction;

    if( asnMsg.m.speedPresent )
        nSpeed_ = asnMsg.speed;

    if( asnMsg.m.campPresent )
        nTeam_ = asnMsg.camp;

    if( asnMsg.m.nature_niveauPresent )
        nLevel_ = (E_NatureLevel)asnMsg.nature_niveau;

    if( asnMsg.m.nature_armePresent )
        nWeapon_ = (E_UnitNatureWeapon)asnMsg.nature_arme;

    if( asnMsg.m.nature_specialisationPresent )
        nSpecialization_ = (E_UnitNatureSpecialization)asnMsg.nature_specialisation;

    if( asnMsg.m.nature_qualificationPresent )
        nQualifier_ = (E_UnitNatureQualifier)asnMsg.nature_qualification;

    if( asnMsg.m.nature_categoriePresent )
        nCategory_ = (E_UnitNatureCategory)asnMsg.nature_categorie;

    if( asnMsg.m.nature_mobilitePresent )
        nMobility_ = (E_UnitNatureMobility)asnMsg.nature_mobilite;

    if( asnMsg.m.nature_pcPresent )
        bIsPC_ = asnMsg.nature_pc;

    if( asnMsg.m.perception_par_compagniePresent )
    {
        automatePerceptionMap_.Set();
        automatePerceptionMap_.Data().clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
        {
            MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.perception_par_compagnie.elem[i].oid_compagnie );
            assert( pAgent );
            automatePerceptionMap_.Data().insert( std::make_pair( pAgent, (E_PerceptionResult)( 3 - asnMsg.perception_par_compagnie.elem[i].identification_level ) ) ); //$$$ DEGUEULASSE
        }
    }

    if( asnMsg.m.pertinencePresent )
        nRelevance_ = asnMsg.pertinence;

    if( asnMsg.m.capacite_missionPresent )
        nCapacity_ = E_UnitCapaciteMission( asnMsg.capacite_mission );

    if( asnMsg.m.prisonnierPresent )
        bPrisonner_ = asnMsg.prisonnier;

    if( asnMsg.m.renduPresent )
        bSurrendered_ = asnMsg.rendu;

    if( asnMsg.m.refugie_pris_en_comptePresent )
        bRefugies_ = asnMsg.refugie_pris_en_compte;
}
