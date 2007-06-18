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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionPopulation_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 7/03/05 11:46 $
// $Revision: 6 $
// $Workfile: AGR_MOS_MissionPopulation_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MissionPopulation.h"

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
#include "MOS_MTH_LocationList_ListView.h"
#include "MOS_MTH_PointList_ListView.h"
#include "MOS_MTH_Obstacle_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_MTH_ListPoint.h"
                       
using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation constructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPopulation::MOS_MissionPopulation( E_MissionID nTypeMission, QWidget* pParent )
    : MOS_Mission_ABC( nTypeMission, pParent )
{
    pASNMsgOrder_ = new MOS_ASN_MsgPopulationOrder();
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation destructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPopulation::~MOS_MissionPopulation()
{
    delete pASNMsgOrder_;
}

//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::Initialize()
{
    assert( pASNMsgOrder_ );
    MOS_Mission_ABC::Initialize();

    //--------------------
    // Common
    pCommonWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pCommonWidget_, "Common" );
    pCommonLayout_ = new QVBoxLayout( pCommonWidget_  );

    QHBoxLayout* pTmpLayout = new QHBoxLayout( pCommonLayout_ );

    pTmpLayout = new QHBoxLayout( pCommonLayout_ );

    CreateMissionTools();

    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::Terminate()
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
// Name: MOS_MissionPopulation::SetAgent
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::SetAgent( MOS_Agent* pAgent )
{
    MOS_Mission_ABC::SetAgent( pAgent );
    if( pAgent )
    {
        pASNMsgOrder_->GetAsnMsg().oid_unite_executante = pAgent->GetAgentID();
        pASNMsgOrder_->GetAsnMsg().order_id = pAgent->GetAgentID();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::CreateMissionTools
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::CreateMissionTools()
{
    switch( nTypeMission_ )
    {
$MosMissionCreation$
        default :    
           assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::SendMsg
// Created: 
// -----------------------------------------------------------------------------
void MOS_MissionPopulation::SendMsg( std::string& sParam )
{
    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->WriteMsg( sParam );

    pASNMsgOrder_->Send( 45 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::FillRandomParameters()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPopulation::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::IsAutomateMission
/** @return 
*/
// Created: 
// -----------------------------------------------------------------------------
bool MOS_MissionPopulation::IsAutomateMission() const
{
    return false;
}


// =============================================================================
// Missions
// =============================================================================
$MissionCreatorImplementations$

// -----------------------------------------------------------------------------
// Name: MOS_MissionPopulation::GetName()
// Created: SBO 2005-08-01
// -----------------------------------------------------------------------------
std::string MOS_MissionPopulation::GetName() const
{
    return ENT_Tr::ConvertFromPopulationMission( ( E_PopulationMission )nTypeMission_ );
}