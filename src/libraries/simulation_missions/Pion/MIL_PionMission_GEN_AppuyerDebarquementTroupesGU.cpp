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
#include "MIL_PionMission_GEN_AppuyerDebarquementTroupesGU.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::nDIAZoneAppuiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneAppuiIdx_ = DEC_Tools::InitializeDIAField( "zoneAppui_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::MIL_PionMission_GEN_AppuyerDebarquementTroupesGU( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::~MIL_PionMission_GEN_AppuyerDebarquementTroupesGU()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *asnMsg.mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_appui, GetVariable( nDIAZoneAppuiIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_AppuyerDebarquementTroupesGU& mission = static_cast< MIL_PionMission_GEN_AppuyerDebarquementTroupesGU& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneAppuiIdx_ ), GetVariable( nDIAZoneAppuiIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *new ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu;
    asnMsg.mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneAppuiIdx_ ), asnMission.zone_appui );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AppuyerDebarquementTroupesGU::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu );
    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *asnMsg.mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu;

    NET_ASN_Tools::Delete( asnMission.zone_appui );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
