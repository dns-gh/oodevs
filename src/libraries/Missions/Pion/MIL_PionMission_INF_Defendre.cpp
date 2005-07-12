// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_INF_Defendre.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Defendre::nDIAPointADefendreIdx_ = 0 ;
int MIL_PionMission_INF_Defendre::nDIAPreparerTerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Defendre::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointADefendreIdx_ = DEC_Tools::InitializeDIAField( "pointADefendre_", diaType );
    nDIAPreparerTerrainIdx_ = DEC_Tools::InitializeDIAField( "preparerTerrain_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Defendre::MIL_PionMission_INF_Defendre( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Defendre::~MIL_PionMission_INF_Defendre()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Defendre::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Defendre& asnMission = *asnMsg.mission.u.mission_pion_inf_defendre;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_a_defendre, pointADefendre_, GetVariable( nDIAPointADefendreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.preparer_terrain, GetVariable( nDIAPreparerTerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Defendre::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointADefendre_, GetVariable( nDIAPointADefendreIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPreparerTerrainIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Defendre::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Defendre& mission = static_cast< MIL_PionMission_INF_Defendre& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointADefendreIdx_ ), pointADefendre_, GetVariable( nDIAPointADefendreIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAPreparerTerrainIdx_ ), GetVariable( nDIAPreparerTerrainIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Defendre::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPreparerTerrainIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Defendre::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Defendre& asnMission = *new ASN1T_Mission_Pion_INF_Defendre();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_defendre;
    asnMsg.mission.u.mission_pion_inf_defendre  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointADefendreIdx_ ), asnMission.point_a_defendre );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPreparerTerrainIdx_ ), asnMission.preparer_terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Defendre::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Defendre::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_defendre );
    ASN1T_Mission_Pion_INF_Defendre& asnMission = *asnMsg.mission.u.mission_pion_inf_defendre;

    NET_ASN_Tools::Delete( asnMission.point_a_defendre );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
