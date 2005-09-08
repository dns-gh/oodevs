// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:24 $
// $Revision: 8 $
// $Workfile: AGR_MOS_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MissionPion.h"

#include "MOS_LimaMission.h"
#include "MOS_LimitMission.h"
#include "MOS_MTH_Itineraire1Pos.h"
#include "MOS_MTH_ItinerairePos.h"
#include "MOS_AgentListView.h"
#include "MOS_AgentMission.h"
#include "MOS_ValueList.h"
#include "MOS_MTH_SegmentPos.h"
#include "MOS_MTH_DirectionPos.h"
#include "MOS_MTH_AgentKnowledgeList.h"
#include "MOS_MTH_ObjectKnowledgeList.h"
#include "MOS_MTH_GDH.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_PointListViewItem.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_ListLocalisation_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_MTH_ListPoint.h"
#include "MOS_MTH_PointList_ListView.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion constructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPion::MOS_MissionPion( E_MissionID nTypeMission, QWidget* pParent )
    : MOS_Mission_ABC( nTypeMission, pParent )
{
    pASNMsgOrder_ = new MOS_ASN_MsgPionOrder();
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion destructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPion::~MOS_MissionPion()
{
    delete pASNMsgOrder_;
}

//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::Initialize
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::Initialize()
{
    MOS_Mission_ABC::Initialize();
   

    //--------------------
    // Lima / limit
    pLimaLimitWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pLimaLimitWidget_, "Limas / Limites" );
    pLimaLimitLayout_ = new QVBoxLayout( pLimaLimitWidget_ );

    CreateLimitMission( pASNMsgOrder_->GetAsnMsg().oid_limite_gauche, pASNMsgOrder_->GetAsnMsg().oid_limite_droite, pLimaLimitWidget_, pLimaLimitLayout_, "Limits" );
    CreateLimaMission ( pASNMsgOrder_->GetAsnMsg().oid_limas, pLimaLimitWidget_, pLimaLimitLayout_, "Limas" ); 

    //--------------------
    // Common
    pCommonWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pCommonWidget_, "Common" );
    pCommonLayout_ = new QVBoxLayout( pCommonWidget_  );


    // Direction dangereuse
    CreateDirection( pASNMsgOrder_->GetAsnMsg().direction_dangereuse, pCommonWidget_, pCommonLayout_, "Danger", true );

    CreateMissionTools();

    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::Terminate()
{                                
    MOS_Mission_ABC::Terminate();
    
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
$AsnMissionDeletion$
        default:
            assert( false );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::SetAgent
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::SetAgent( MOS_Agent* pAgent )
{
    MOS_Mission_ABC::SetAgent( pAgent );
    if( pAgent )
    {
        pASNMsgOrder_->GetAsnMsg().order_id = (uint)GetAgent();
        pASNMsgOrder_->GetAsnMsg().oid_unite_executante = pAgent->GetAgentID();

    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::CreateMissionTools
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::CreateMissionTools()
{
    switch( nTypeMission_ )
    {

$MosMissionCreation$

        default:
            assert( false );
    }
}


//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::WriteOrder
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::SendMsg( std::string& sParam )
{
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        sParam += "------------------------------------------------|";
        (*itMissionTool)->WriteMsg( sParam );
    }

    if( pASNMsgOrder_->GetAsnMsg().oid_limite_gauche != MIL_NULL_LIMIT_ID )
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_gauchePresent  = 1;
    else
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_gauchePresent  = 0;

    if( pASNMsgOrder_->GetAsnMsg().oid_limite_droite != MIL_NULL_LIMIT_ID )
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_droitePresent  = 1;
    else
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_droitePresent  = 0;

      
    pASNMsgOrder_->Send( 45 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::FillRandomParameters()
{
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->FillRandomParameters();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint i = 0;
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        (*itMissionTool)->ReloadParameters( archive );
        ++i;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::ResaveParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint i = 0;
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        (*itMissionTool)->ResaveParameters( archive );
        ++i;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::IsAutomateMission
/** @return 
*/
// Created: 
// -----------------------------------------------------------------------------
bool MOS_MissionPion::IsAutomateMission() const
{
    return false;
}

/*
//-----------------------------------------------------------------------------
// Name: void MOS_MissionPion::CreateMission_Test_MoveTo
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_MissionPion::CreateMission_Test_MoveTo()
{
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_move_to;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to  = &asnMission;

    MOS_MTH_ItinerairePos& itineraire = CreatePath( asnMission.itineraire, "Itineraire" );
    itineraire.SetFirstWidget( true );

    MOS_ValueList* pPathFind = &CreateValueList( "Mode", 2, 2 );
    pPathFind->CreateValue( asnMission.mode_short      , "short" , MOSToolValueCheck | MOSToolValueModif );
    pPathFind->CreateValue( asnMission.mode_speed      , "speed" , MOSToolValueCheck | MOSToolValueModif );

    MOS_ValueList* pPathIgnore = &CreateValueList( "Ignore", 2, 2 );
    pPathIgnore->CreateValue( asnMission.ign_road          , "road"     , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_crossroad     , "crossroad", MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_forest        , "forest"   , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_urban         , "urban"    , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_river         , "river"    , MOSToolValueCheck | MOSToolValueModif );

    MOS_ValueList* pPathInfos = &CreateValueList( "Infos", 3, 2 );
    pPathInfos->CreateValue( asnMission.inf_object        , "object"     , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_eni_avoid     , "eni avoid"  , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_eni_attack    , "eni attack" , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_fuseau        , "fuseau"     , MOSToolValueCheck | MOSToolValueModif );

    CreatePoint( asnMission.point_dir_locked, "Point/direction lockée" );

    MOS_ValueButtonList<ASN1T_Mission_Pion_Test_MoveTo_lock>*  pLockSelector = &CreateVarList( asnMission.lock, "Lock", 1, 3, false );
    pLockSelector->CreateValue( "Rien",       Mission_Pion_Test_MoveTo_lock::rien, true );
    pLockSelector->CreateValue( "Point",      Mission_Pion_Test_MoveTo_lock::point );
    pLockSelector->CreateValue( "Direction",  Mission_Pion_Test_MoveTo_lock::direction );

    MOS_ValueList* pTmp = &CreateValueList( "Misc", 2, 2 );
    pTmp->CreateValue( asnMission.mode_surete, "surete"  , MOSToolValueCheck | MOSToolValueModif );
    pTmp->CreateValue( asnMission.debarque   , "debarque", MOSToolValueCheck | MOSToolValueModif );
}
*/

// =============================================================================
// Missions
// =============================================================================
$MissionCreatorImplementations$

// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::GetName()
// Created: SBO 2005-08-01
// -----------------------------------------------------------------------------
std::string MOS_MissionPion::GetName() const
{
    return ENT_Tr::ConvertFromUnitMission( ( E_UnitMission )nTypeMission_ );
}
