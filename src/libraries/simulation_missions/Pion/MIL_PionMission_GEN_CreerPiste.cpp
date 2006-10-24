// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_PionMission_GEN_CreerPiste.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_CreerPiste::nDIAPosObstacleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_CreerPiste::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_CreerPiste::MIL_PionMission_GEN_CreerPiste( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_CreerPiste::~MIL_PionMission_GEN_CreerPiste()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_CreerPiste::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *asnMsg.mission.u.mission_pion_gen_creer_piste;
    if( !NET_ASN_Tools::CopyLocation( asnMission.pos_obstacle, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_CreerPiste::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_CreerPiste::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_CreerPiste& mission = static_cast< MIL_PionMission_GEN_CreerPiste& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAPosObstacleIdx_ ), GetVariable( nDIAPosObstacleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_CreerPiste::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_CreerPiste::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPiste();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_creer_piste;
    asnMsg.mission.u.mission_pion_gen_creer_piste  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_CreerPiste::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_CreerPiste::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_creer_piste );
    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *asnMsg.mission.u.mission_pion_gen_creer_piste;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
