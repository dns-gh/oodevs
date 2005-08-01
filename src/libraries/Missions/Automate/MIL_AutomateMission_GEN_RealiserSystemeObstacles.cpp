// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_GEN_RealiserSystemeObstacles.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserSystemeObstacles::nDIAObstaclesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserSystemeObstacles::MIL_AutomateMission_GEN_RealiserSystemeObstacles( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserSystemeObstacles::~MIL_AutomateMission_GEN_RealiserSystemeObstacles()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserSystemeObstacles::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstaclesIdx_ = DEC_Tools::InitializeDIAField( "obstacles_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserSystemeObstacles::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_systeme_obstacles;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.obstacles, GetVariable( nDIAObstaclesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserSystemeObstacles::Terminate()
{
    NET_ASN_Tools::ResetGenObjectList( GetVariable( nDIAObstaclesIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserSystemeObstacles::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles;
    asnMsg.mission.u.mission_automate_gen_realiser_systeme_obstacles  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIAObstaclesIdx_ ), asnMission.obstacles );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserSystemeObstacles::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserSystemeObstacles::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles );
    ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_systeme_obstacles;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
