// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_GEN_DegagerObstaclesDansZone.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_DegagerObstaclesDansZone::nDIAObstaclesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_DegagerObstaclesDansZone::MIL_AutomateMission_GEN_DegagerObstaclesDansZone( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_DegagerObstaclesDansZone::~MIL_AutomateMission_GEN_DegagerObstaclesDansZone()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_DegagerObstaclesDansZone::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstaclesIdx_ = DEC_Tools::InitializeDIAField( "obstacles_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_DegagerObstaclesDansZone::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone& asnMission = *asnMsg.mission.u.mission_automate_gen_degager_obstacles_dans_zone;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.obstacles, GetVariable( nDIAObstaclesIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_DegagerObstaclesDansZone::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_DegagerObstaclesDansZone::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone& asnMission = *new ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_degager_obstacles_dans_zone;
    asnMsg.mission.u.mission_automate_gen_degager_obstacles_dans_zone  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAObstaclesIdx_ ), asnMission.obstacles, automate_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_DegagerObstaclesDansZone::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_DegagerObstaclesDansZone::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_degager_obstacles_dans_zone );
    ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone& asnMission = *asnMsg.mission.u.mission_automate_gen_degager_obstacles_dans_zone;

    NET_ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
