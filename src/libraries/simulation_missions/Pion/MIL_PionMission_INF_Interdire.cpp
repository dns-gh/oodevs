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
#include "MIL_PionMission_INF_Interdire.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Interdire::nDIAPointAInterdireIdx_ = 0 ;
int MIL_PionMission_INF_Interdire::nDIAPreparerTerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Interdire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointAInterdireIdx_ = DEC_Tools::InitializeDIAField( "pointAInterdire_", diaType );
    nDIAPreparerTerrainIdx_ = DEC_Tools::InitializeDIAField( "preparerTerrain_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Interdire::MIL_PionMission_INF_Interdire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Interdire::~MIL_PionMission_INF_Interdire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Interdire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Interdire& asnMission = *asnMsg.mission.u.mission_pion_inf_interdire;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_a_interdire, GetVariable( nDIAPointAInterdireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.preparer_terrain, GetVariable( nDIAPreparerTerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Interdire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Interdire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Interdire& mission = static_cast< MIL_PionMission_INF_Interdire& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointAInterdireIdx_ ), GetVariable( nDIAPointAInterdireIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAPreparerTerrainIdx_ ), GetVariable( nDIAPreparerTerrainIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Interdire::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Interdire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Interdire& asnMission = *new ASN1T_Mission_Pion_INF_Interdire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_interdire;
    asnMsg.mission.u.mission_pion_inf_interdire  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointAInterdireIdx_ ), asnMission.point_a_interdire );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPreparerTerrainIdx_ ), asnMission.preparer_terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Interdire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Interdire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_interdire );
    ASN1T_Mission_Pion_INF_Interdire& asnMission = *asnMsg.mission.u.mission_pion_inf_interdire;

    NET_ASN_Tools::Delete( asnMission.point_a_interdire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
