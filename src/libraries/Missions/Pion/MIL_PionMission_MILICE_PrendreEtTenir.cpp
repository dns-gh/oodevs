// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_MILICE_PrendreEtTenir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_MILICE_PrendreEtTenir::nDIAPointAPrendreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_MILICE_PrendreEtTenir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointAPrendreIdx_ = DEC_Tools::InitializeDIAField( "pointAPrendre_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_MILICE_PrendreEtTenir::MIL_PionMission_MILICE_PrendreEtTenir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_MILICE_PrendreEtTenir::~MIL_PionMission_MILICE_PrendreEtTenir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_MILICE_PrendreEtTenir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_MILICE_PrendreEtTenir& asnMission = *asnMsg.mission.u.mission_pion_milice_prendre_et_tenir;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_a_prendre, pointAPrendre_, GetVariable( nDIAPointAPrendreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_MILICE_PrendreEtTenir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointAPrendre_, GetVariable( nDIAPointAPrendreIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_MILICE_PrendreEtTenir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_MILICE_PrendreEtTenir& mission = static_cast< MIL_PionMission_MILICE_PrendreEtTenir& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointAPrendreIdx_ ), pointAPrendre_, GetVariable( nDIAPointAPrendreIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_MILICE_PrendreEtTenir::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointAPrendre_, GetVariable( nDIAPointAPrendreIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_MILICE_PrendreEtTenir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_MILICE_PrendreEtTenir& asnMission = *new ASN1T_Mission_Pion_MILICE_PrendreEtTenir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_milice_prendre_et_tenir;
    asnMsg.mission.u.mission_pion_milice_prendre_et_tenir  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointAPrendreIdx_ ), asnMission.point_a_prendre );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_MILICE_PrendreEtTenir::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_MILICE_PrendreEtTenir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_milice_prendre_et_tenir );
    ASN1T_Mission_Pion_MILICE_PrendreEtTenir& asnMission = *asnMsg.mission.u.mission_pion_milice_prendre_et_tenir;

    NET_ASN_Tools::Delete( asnMission.point_a_prendre );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
