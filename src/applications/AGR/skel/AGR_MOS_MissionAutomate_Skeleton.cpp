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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 7/03/05 11:46 $
// $Revision: 6 $
// $Workfile: AGR_MOS_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MissionAutomate.h"

#include "MOS_LimaMission.h"
#include "MOS_LimitMission.h"
#include "MOS_MTH_Obstacle.h"
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
#include "MOS_MTH_PointList_ListView.h"
#include "MOS_MTH_Obstacle_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_MTH_ListPoint.h"
                       
using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate constructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionAutomate::MOS_MissionAutomate( E_MissionID nTypeMission, QWidget* pParent )
    : MOS_Mission_ABC( nTypeMission, pParent )
{
    pASNMsgOrder_ = new MOS_ASN_MsgAutomateOrder();
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate destructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionAutomate::~MOS_MissionAutomate()
{
    delete pASNMsgOrder_;
}

//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::Initialize
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::Initialize()
{
    assert( pASNMsgOrder_ );
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

    QHBoxLayout* pTmpLayout = new QHBoxLayout( pCommonLayout_ );

    pTmpLayout = new QHBoxLayout( pCommonLayout_ );

    // Formation
    MOS_ValueButtonList<ASN1T_EnumAutomateOrderFormation>* pFormation = &CreateVarList( pASNMsgOrder_->GetAsnMsg().formation, pCommonWidget_, pTmpLayout, "Formation", 1, 3, true  );
    pFormation->CreateValue( "Un échelon"   , EnumAutomateOrderFormation::un_echelon, true );
    pFormation->CreateValue( "Deux échelons", EnumAutomateOrderFormation::deux_echelons );

    // Direction ennemi
    CreateDirection( pASNMsgOrder_->GetAsnMsg().direction_dangereuse, pCommonWidget_, pCommonLayout_, "Direction dangereuse", true );

    CreateMissionTools();

    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::Terminate()
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
// Name: MOS_MissionAutomate::SetAgent
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::SetAgent( MOS_Agent* pAgent )
{
    MOS_Mission_ABC::SetAgent( pAgent );
    if( pAgent )
    {
        pASNMsgOrder_->GetAsnMsg().oid_unite_executante = pAgent->GetAgentID();
        pASNMsgOrder_->GetAsnMsg().order_id = pAgent->GetAgentID();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::CreateMissionTools
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::CreateMissionTools()
{
    switch( nTypeMission_ )
    {
$MosMissionCreation$
        default :    
           assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::SendMsg
// Created: 
// -----------------------------------------------------------------------------
void MOS_MissionAutomate::SendMsg( std::string& sParam )
{
    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->WriteMsg( sParam );

    pASNMsgOrder_->Send( 45 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::FillRandomParameters()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionAutomate::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::IsAutomateMission
/** @return 
*/
// Created: 
// -----------------------------------------------------------------------------
bool MOS_MissionAutomate::IsAutomateMission() const
{
    return true;
}


// =============================================================================
// Missions
// =============================================================================
$MissionCreatorImplementations$

// -----------------------------------------------------------------------------
// Name: MOS_MissionAutomate::GetName()
// Created: SBO 2005-08-01
// -----------------------------------------------------------------------------
std::string MOS_MissionAutomate::GetName() const
{
    return ENT_Tr::ConvertFromAutomataMission( ( E_AutomataMission )nTypeMission_ );
}