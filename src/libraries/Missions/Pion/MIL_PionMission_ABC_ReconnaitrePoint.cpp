// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ABC_ReconnaitrePoint.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_ReconnaitrePoint::nDIAPointAReconnaitreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_ReconnaitrePoint::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointAReconnaitreIdx_ = DEC_Tools::InitializeDIAField( "pointAReconnaitre_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ReconnaitrePoint::MIL_PionMission_ABC_ReconnaitrePoint( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ReconnaitrePoint::~MIL_PionMission_ABC_ReconnaitrePoint()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_ReconnaitrePoint::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_pion_abc_reconnaitre_point;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_a_reconnaitre, pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ReconnaitrePoint::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ReconnaitrePoint::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_ReconnaitrePoint& mission = static_cast< MIL_PionMission_ABC_ReconnaitrePoint& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointAReconnaitreIdx_ ), pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitrePoint::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitrePoint::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitrePoint();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_reconnaitre_point;
    asnMsg.mission.u.mission_pion_abc_reconnaitre_point  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointAReconnaitreIdx_ ), asnMission.point_a_reconnaitre );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitrePoint::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitrePoint::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_reconnaitre_point );
    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_pion_abc_reconnaitre_point;

    NET_ASN_Tools::Delete( asnMission.point_a_reconnaitre );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
