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
#include "MIL_PionMission_INF_Couvrir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Couvrir::nDIAPointCouvertureIdx_ = 0 ;
int MIL_PionMission_INF_Couvrir::nDIACibleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Couvrir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointCouvertureIdx_ = DEC_Tools::InitializeDIAField( "pointCouverture_", diaType );
    nDIACibleIdx_ = DEC_Tools::InitializeDIAField( "cible_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Couvrir::MIL_PionMission_INF_Couvrir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Couvrir::~MIL_PionMission_INF_Couvrir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Couvrir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Couvrir& asnMission = *asnMsg.mission.u.mission_pion_inf_couvrir;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_couverture, pointCouverture_, GetVariable( nDIAPointCouvertureIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgent( asnMission.cible, GetVariable( nDIACibleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Couvrir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointCouverture_, GetVariable( nDIAPointCouvertureIdx_ ) );
    NET_ASN_Tools::ResetAgent( GetVariable( nDIACibleIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Couvrir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Couvrir& mission = static_cast< MIL_PionMission_INF_Couvrir& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointCouvertureIdx_ ), pointCouverture_, GetVariable( nDIAPointCouvertureIdx_ ) );
    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIACibleIdx_ ), GetVariable( nDIACibleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Couvrir::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIACibleIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Couvrir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *new ASN1T_Mission_Pion_INF_Couvrir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_couvrir;
    asnMsg.mission.u.mission_pion_inf_couvrir  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointCouvertureIdx_ ), asnMission.point_couverture );
    NET_ASN_Tools::CopyAgent( GetVariable( nDIACibleIdx_ ), asnMission.cible );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Couvrir::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Couvrir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_couvrir );
    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *asnMsg.mission.u.mission_pion_inf_couvrir;

    NET_ASN_Tools::Delete( asnMission.point_couverture );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
