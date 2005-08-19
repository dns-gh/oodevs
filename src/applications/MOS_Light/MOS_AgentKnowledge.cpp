//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentKnowledge.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 16:30 $
// $Revision: 21 $
// $Workfile: MOS_AgentKnowledge.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_AgentKnowledge.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentKnowledge.inl"
#endif

#include "GFX/GFX_Color.h"
#include "MOS_App.h"
#include "MOS_WorldWidget.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_ASN_Messages.h"
#include "MOS_AgentKnowledge_Editor.h"
#include "MOS_EnemyInfo_Localisation.h"
#include "MOS_EnemyInfo_Direction.h"
#include "MOS_PointListViewItem.h"
#include "MOS_Value.h"
#include "MOS_Tools.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"

MOS_IDManager MOS_AgentKnowledge::idManager_( 158 );



// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentKnowledge::MOS_AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
    : nID_                    ( asnMsg.oid_connaissance )
    , pAgent_                 ( MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_unite_reelle ) )
    , vPosition_              ( )
    , nDirection_             ( 0 ) 
    , nSpeed_                 ( uint( -1 ) )
    , nEtatOps_               ( 0 )
    , nCurrentPerceptionLevel_( eNotSeen )
    , nMaxPerceptionLevel_    ( eNotSeen )
    , nTeam_                  ( uint( -1 ) )
    , nLevel_                 ( eNatureLevel_None )
    , nWeapon_                ( eUnitNatureWeapon_None )
    , nSpecialization_        ( eUnitNatureSpecialization_None )
    , nQualifier_             ( eUnitNatureQualifier_None )
    , nCategory_              ( eUnitNatureCategory_None )
    , nMobility_              ( eUnitNatureMobility_None )
    , bIsPC_                  ( false )
    , nAttrUpdated_           ( 0 )
    , pEditor_                ( 0 )
    , bSurrendered_           ( false )
    , bPrisoner_              ( false )
    , bRefugeesManaged_       ( false )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentKnowledge::~MOS_AgentKnowledge()
{
    if( pEditor_ )
        pEditor_->SlotValidate();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetAgentID
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
uint MOS_AgentKnowledge::GetAgentID() const
{
    if( pAgent_ )
        return pAgent_->GetAgentID();
    return uint( -1 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetName
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_AgentKnowledge::GetName() const
{
    if( pAgent_ )
        return pAgent_->GetName();
    return "HLA unit";
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentKnowledge::Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.identification_levelPresent )
    {
        nCurrentPerceptionLevel_ = (E_PerceptionResult)( 3 - asnMsg.identification_level ); //$$$ DEGUEULASSE
        nAttrUpdated_ |= eUpdated_CurrentPerceptionLevel;
    }

    if( asnMsg.m.max_identification_levelPresent )
    {
        nMaxPerceptionLevel_ = (E_PerceptionResult)( 3 - asnMsg.max_identification_level );
        nAttrUpdated_ |= eUpdated_MaxPerceptionLevel;
    }

    if( asnMsg.m.etat_opPresent )
    {
        nEtatOps_ = asnMsg.etat_op;
        nAttrUpdated_ |= eUpdated_EtatOps;
    }

    if( asnMsg.m.positionPresent )
    {
        strPosition_ = std::string( (const char*)asnMsg.position.data, 15 );
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, vPosition_ );
        nAttrUpdated_ |= eUpdated_Position;
    }

    if( asnMsg.m.directionPresent )
    {
        nDirection_ = asnMsg.direction;
        nAttrUpdated_ |= eUpdated_Direction;
    }

    if( asnMsg.m.speedPresent )
    {
        nSpeed_ = asnMsg.speed;
        nAttrUpdated_ |= eUpdated_Speed;
    }

    if( asnMsg.m.campPresent )
    {
        nTeam_ = asnMsg.camp;
        nAttrUpdated_ |= eUpdated_Team;
    }

    if( asnMsg.m.nature_niveauPresent )
    {
        nLevel_ = (E_NatureLevel)asnMsg.nature_niveau;
        nAttrUpdated_ |= eUpdated_Level;
    }

    if( asnMsg.m.nature_armePresent )
    {
        nWeapon_ = (E_UnitNatureWeapon)asnMsg.nature_arme;
        nAttrUpdated_ |= eUpdated_Weapon;
    }

    if( asnMsg.m.nature_specialisationPresent )
    {
        nSpecialization_ = (E_UnitNatureSpecialization)asnMsg.nature_specialisation;
        nAttrUpdated_ |= eUpdated_Specialization;
    }

    if( asnMsg.m.nature_qualificationPresent )
    {
        nQualifier_ = (E_UnitNatureQualifier)asnMsg.nature_qualification;
        nAttrUpdated_ |= eUpdated_Qualifier;
    }

    if( asnMsg.m.nature_categoriePresent )
    {
        nCategory_ = (E_UnitNatureCategory)asnMsg.nature_categorie;
        nAttrUpdated_ |= eUpdated_Category;
    }

    if( asnMsg.m.nature_mobilitePresent )
    {
        nMobility_ = (E_UnitNatureMobility)asnMsg.nature_mobilite;
        nAttrUpdated_ |= eUpdated_Mobility;
    }

    if ( asnMsg.m.capacite_missionPresent )
    {
        nMissionCapacity_ = (E_UnitCapaciteMission)asnMsg.capacite_mission;
        nAttrUpdated_ |= eUpdated_MissionCapacity;
    }

    if( asnMsg.m.nature_pcPresent )
    {
        bIsPC_ = asnMsg.nature_pc;
        nAttrUpdated_ |= eUpdated_IsPC;
    }

    if( asnMsg.m.prisonnierPresent )
    {
        bPrisoner_ = asnMsg.prisonnier;
        nAttrUpdated_ |= eUpdated_Prisoner;
    }

    if( asnMsg.m.refugie_pris_en_comptePresent )
    {
        bRefugeesManaged_ = asnMsg.refugie_pris_en_compte;
        nAttrUpdated_ |= eUpdated_RefugeesManaged;
    }

    if( asnMsg.m.renduPresent )
    {
        bSurrendered_ = asnMsg.rendu;
        nAttrUpdated_ |= eUpdated_Surrendered;
    }

    if( asnMsg.m.perception_par_compagniePresent )
    {
        nAttrUpdated_ |= eUpdated_AutomatePerception;
        automatePerceptionMap_.clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
        {
            MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.perception_par_compagnie.elem[i].oid_compagnie );
            if( pAgent )
                automatePerceptionMap_.insert( std::make_pair( pAgent, (E_PerceptionResult)( 3 - asnMsg.perception_par_compagnie.elem[i].identification_level ) ) );
        }
    }

    if( asnMsg.m.pertinencePresent )
    {
        nRelevance_ = asnMsg.pertinence;
        nAttrUpdated_ |= eUpdated_Relevance;
    }

    if( pEditor_ )
        pEditor_->Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::SetEditor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentKnowledge::SetEditor( MOS_AgentKnowledge_Editor* pEditor )
{
    pEditor_ = pEditor;
    if( pEditor_ )
        pEditor_->Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::Draw
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentKnowledge::Draw()
{
/*    MT_Float rSize = 600. / MOS_App::GetApp().GetWorld().GetExtent().GetMeterPerPixel();
    switch( nCurrentPerceptionLevel_ )
    {
    case eIdentification:
        {
            GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, symbolName_, 100.0 );
            break;
        }
    case eRecognition:
        {
            GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, symbolName_, 66.0 );
            break;
        }
    case eDetection:
        {
            GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, symbolName_, 33.0 );
            break;
        }
    case eNotSeen:
        {
            GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, symbolName_, 0.0 );
            break;
        }

    }
    */
    MT_Float rCrossSize = 100.;
    GFX_Tools::CreateGLCross( vPosition_, rCrossSize, 4.0, GFX_Color( 255, 255, 255 ) );
}
