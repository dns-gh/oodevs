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
#include "MIL_PionMission_INF_ReconnaitrePoint.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_ReconnaitrePoint::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_INF_ReconnaitrePoint::nDIAPointAReconnaitreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_ReconnaitrePoint::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAPointAReconnaitreIdx_ = DEC_Tools::InitializeDIAField( "pointAReconnaitre_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ReconnaitrePoint::MIL_PionMission_INF_ReconnaitrePoint( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ReconnaitrePoint::~MIL_PionMission_INF_ReconnaitrePoint()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_ReconnaitrePoint::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_pion_inf_reconnaitre_point;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_a_reconnaitre, GetVariable( nDIAPointAReconnaitreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_ReconnaitrePoint::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_ReconnaitrePoint::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_ReconnaitrePoint& mission = static_cast< MIL_PionMission_INF_ReconnaitrePoint& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointAReconnaitreIdx_ ), GetVariable( nDIAPointAReconnaitreIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ReconnaitrePoint::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ReconnaitrePoint::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitrePoint();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_reconnaitre_point;
    asnMsg.mission.u.mission_pion_inf_reconnaitre_point  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointAReconnaitreIdx_ ), asnMission.point_a_reconnaitre );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ReconnaitrePoint::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ReconnaitrePoint::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_reconnaitre_point );
    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_pion_inf_reconnaitre_point;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.point_a_reconnaitre );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
